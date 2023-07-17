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

    message = (char *)tqueue_pop(connection_args->queue);

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

  tqueue_destroy((void **)&connection_args->queue);
  close(connection_args->client_socket);
  free(connection_args);

  return NULL;
}

void *ircd_connection_receive(void *args)
{
  char buffer[BUFFER_SIZE];
  int bytes_received;
  connection_thread_args_t *connection_args = (connection_thread_args_t *)args;

  while (1)
  {
    memset(buffer, 0, sizeof(buffer));
    if ((bytes_received = read(connection_args->client_socket, buffer, BUFFER_SIZE)) > 0)
    {
      if (buffer[0] == '/')
      {
        if (strcmp(buffer, "/quit") == 0)
        {
          char *quit_message = malloc(6);
          strcpy(quit_message, buffer);
          tqueue_push(connection_args->queue, quit_message);
          break;
        }
        else if (strcmp(buffer, "/ping") == 0)
        {
          send(connection_args->client_socket, "pong", strlen("pong"), 0);
          continue;
        }
        else
        {
          send(connection_args->client_socket, "Invalid command!", strlen("Invalid command!"), 0);
          continue;
        }
      }

      for (int i = 0; i < tlist_get_size(connection_args->tlist); i++)
      {
        tqueue_t *queue = (tqueue_t *)tlist_get(connection_args->tlist, i);

        char *message = (char *)malloc(strlen(buffer) + 51);

        sprintf(message, "%s: %s", connection_args->nickname, buffer);

        tqueue_push(queue, (void **)message);
      }
    }
  }

  // FILE *fp = fopen("/home/lucas/buffer/irc-network/teste.txt", "w");
  // fprintf(fp, "DEU QUIT RECEIVE");
  // fclose(fp);
  tlist_remove(connection_args->tlist, connection_args->queue);

  return NULL;
}

// struct pollfd client_poll;
// memset(&client_poll, 0, sizeof(client_poll));

// client_poll.fd = client_connection.socket;
// client_poll.events = POLLIN;

// int poll_error = poll(&client_poll, 1, -1);

// printf("%d", poll_error);