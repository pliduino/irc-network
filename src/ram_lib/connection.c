#include "connection.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <sys/socket.h>

#include <unistd.h>

void *receive_thread(void *vargp) {
  char buffer[BUFFER_SIZE] = {0};
  int bytes_received;
  int new_socket = *((int *)vargp);

  while (1) {
    memset(buffer, 0, sizeof(buffer));
    if ((bytes_received = read(new_socket, buffer, BUFFER_SIZE)) > 0) {
      printf("%s: %s\n", "Other", buffer);
    }
  }
}

void *send_thread(void *vargp) {
  char message[BUFFER_SIZE];
  int sockfd = *((int *)vargp);

  while (1) {
    fgets(message, BUFFER_SIZE, stdin);

    message[strlen(message) - 1] = '\0';

    if (strcmp(message, "/quit") == 0) {
      break;
    }

    send(sockfd, message, strlen(message), 0);
  }
}
