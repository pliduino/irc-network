#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <netinet/in.h>
#include <sys/socket.h>

#include "internal.h"

#define BUFFER_SIZE 4096

char **readMessage(int *tam, int buffer, FILE *stream)
{
  char **message = 0;
  char c;
  fscanf(stream, " %c", &c);
  for (*tam = 0; 1; *tam += 1)
  {
    message = (char **)realloc(message, (*tam + 1) * sizeof(char *));
    message[*tam] = (char *)malloc(buffer);

    for (int i = 0; i < buffer - 1; i++)
    {
      if (c == '\n' || c == '\r' || c == EOF)
      {
        message[*tam][i] = '\0';
        *tam += 1;
        return message;
      }
      message[*tam][i] = c;
      c = fgetc(stream);
    }
    message[*tam][buffer - 1] = '\0';
  }
}

void *ircd_connection_send(void *args)
{
  char **message;
  int sockfd = *(int *)args;

  while (1)
  {
    int tam;
    message = readMessage(&tam, BUFFER_SIZE, stdin);

    if (strcmp(message[0], "/quit") == 0)
    {
      break;
    }
    for (int i = 0; i < tam; i++)
    {
      send(sockfd, message[i], strlen(message[i]), 0);
      free(message[i]);
    }
    free(message);
  }
  return NULL;
}

void *ircd_connection_receive(void *args)
{
  char buffer[BUFFER_SIZE] = {0};
  int bytes_received;
  int new_socket = *(int *)args;

  while (1)
  {
    memset(buffer, 0, sizeof(buffer));
    if ((bytes_received = read(new_socket, buffer, BUFFER_SIZE)) > 0)
    {
      printf("%s: %s\n", "Other", buffer);
    }
  }
  return NULL;
}

// struct pollfd client_poll;
// memset(&client_poll, 0, sizeof(client_poll));

// client_poll.fd = client_connection.socket;
// client_poll.events = POLLIN;

// int poll_error = poll(&client_poll, 1, -1);

// printf("%d", poll_error);