#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include "../tqueue/api.h"
#include "../ircd/internal.h"
#include "../tlist/api.h"

#define BUFFER_SIZE 4096

void *ircd_connection_send(void *args)
{
  char *message;
  connection_thread_args_t *connection_args = (connection_thread_args_t *)args;

  while (1)
  {
    message = (char *)tqueue_pop(connection_args->channel_queue->queue);

    if (message == NULL)
    {
      continue;
    }

    send(connection_args->client_socket, message, strlen(message), 0);

    if (strcmp(message, "/quit") == 0)
    {
      free(message);
      break;
    }
    free(message);
  }

  tqueue_destroy((void **)&connection_args->channel_queue->queue);
  free(connection_args->channel_queue->channel);
  close(connection_args->client_socket);
  free(connection_args);

  return NULL;
}

void *ircd_connection_receive(void *args)
{
  char buffer[BUFFER_SIZE];
  int8_t is_admin;
  int bytes_received;
  connection_thread_args_t *connection_args = (connection_thread_args_t *)args;

  while (1)
  {
    memset(buffer, 0, sizeof(buffer));
    if ((bytes_received = read(connection_args->client_socket, buffer, BUFFER_SIZE)) > 0)
    {
      if (buffer[0] == '/')
      {
        if (strcmp("/quit", buffer) == 0)
        {
          char *error_message = malloc(6);
          strcpy(error_message, buffer);
          tqueue_push(connection_args->channel_queue->queue, error_message);
          break;
        }
        else if (strcmp("/ping", buffer) == 0)
        {
          send(connection_args->client_socket, "pong", strlen("pong"), 0);
        }
        else if (strncmp("/nickname ", buffer, 10) == 0)
        {
          if (strlen(buffer) > 10)
          {
            strcpy(connection_args->nickname, &buffer[10]);
            int8_t success = 1;

            for (int i = 0; i < tlist_get_size(connection_args->nick_list); i++)
            {
              char *cur_nick = (char *)tlist_get(connection_args->nick_list, i);

              if (strcmp(cur_nick, connection_args->nickname) == 0)
              {
                connection_args->nickname[0] = '\0';
                char *error_message = malloc(strlen("Nickname already exists!") + 1);
                strcpy(error_message, "Nickname already exists!");
                tqueue_push(connection_args->channel_queue->queue, error_message);
                success = 0;
                break;
              }
            }
            if (success == 1)
            {
              tlist_push(connection_args->nick_list, (void *)connection_args->nickname);
            }
          }
        }
        else if (strncmp("/join ", buffer, 6) == 0)
        {
          strcpy(connection_args->channel_queue->channel, &buffer[6]);
          int8_t success = 0;

          for (int i = 0; i < tlist_get_size(connection_args->channel_list); i++)
          {
            char *channel = (char *)tlist_get(connection_args->channel_list, i);

            if (strcmp(channel, connection_args->channel_queue->channel) == 0)
            {
              char *error_message = malloc(strlen("Channel joined!") + 1);
              strcpy(error_message, "Channel joined!");
              tqueue_push(connection_args->channel_queue->queue, error_message);
              is_admin = 0;
              success = 1;
              break;
            }
          }
          if (!success)
          {
            // Create Channel
            if (channel_name_is_valid(connection_args->channel_queue->channel))
            {
              tlist_push(connection_args->channel_list, connection_args->channel_queue->channel);
              is_admin = 1;

              char *error_message = malloc(strlen("Channel created!") + 1);
              strcpy(error_message, "Channel created!");
              tqueue_push(connection_args->channel_queue->queue, error_message);
            }
            else
            {
              char *error_message = malloc(strlen("Invalid channel name") + 1);
              strcpy(error_message, "Invalid channel name");
              tqueue_push(connection_args->channel_queue->queue, error_message);
            }
          }
        }
        else if (is_admin == 1)
        {
          if (strncmp("/mute ", buffer, 6) == 0)
          {
          }
          else if (strncmp("/kick ", buffer, 6) == 0)
          {
          }
          else if (strncmp("/unmute ", buffer, 8) == 0)
          {
          }
          else if (strncmp("/whois ", buffer, 7) == 0)
          {
          }
        }

        else
        {
          send(connection_args->client_socket, "Invalid command!", strlen("Invalid command!"), 0);
        }
        continue;
      }

      if (connection_args->nickname[0] == '\0')
      {
        char *error_message = malloc(strlen("Please set a nickname with /nickname") + 1);
        strcpy(error_message, "Please set a nickname with /nickname");
        tqueue_push(connection_args->channel_queue->queue, error_message);
        continue;
      }

      if (connection_args->channel_queue->channel[0] == '\0')
      {
        char *error_message = malloc(strlen("Please join a channel with /join") + 1);
        strcpy(error_message, "Please join a channel with /join");
        tqueue_push(connection_args->channel_queue->queue, error_message);
        continue;
      }

      for (int i = 0; i < tlist_get_size(connection_args->queue_list); i++)
      {
        channel_queue_t *channel_queue = (channel_queue_t *)tlist_get(connection_args->queue_list, i);

        if (strcmp(connection_args->channel_queue->channel, channel_queue->channel) == 0)
        {
          char *message = (char *)malloc(strlen(buffer) + 51);

          sprintf(message, "%s: %s", connection_args->nickname, buffer);

          tqueue_push(channel_queue->queue, (void **)message);
        }
      }
    }
  }

  tlist_remove(connection_args->queue_list, connection_args->channel_queue);

  return NULL;
}
