#include "server.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <sys/socket.h>

#include <unistd.h>
#define PORT 8080

int server_execute() {
  struct sockaddr_in own_address = {0};

  char buffer[1024] = {0};

  int sockfd;
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("Error while creating the socket\n");
    return -1;
  }

  own_address.sin_family = AF_INET;
  own_address.sin_port = htons(PORT);
  own_address.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(sockfd, (struct sockaddr *)&own_address, sizeof(own_address)) < 0) {
    printf("Error in binding\n");
    return -1;
  }

  listen(sockfd, 3);
  int addrlen;
  struct sockaddr_in client_address;

  printf("**\tWaiting For Client\t**\n");

  int new_socket =
      accept(sockfd, (struct sockaddr *)&client_address, (socklen_t *)&addrlen);

  printf("**\tReceiving Message\t**\n");
  int bytes_received;
  if ((bytes_received = read(new_socket, buffer, 1024)) > 0) {
    printf("%d, %s\n", bytes_received, buffer);
  } else {
    printf("Message too small");
  }

  printf("**\tSending Message\t\t**\n");
  send(new_socket, "Hello Back", 10, 0);

  printf("**\tClosing Connection\t**\n");
  close(new_socket);

  printf("**\tShutting Down Server\t**\n");
  shutdown(sockfd, SHUT_RDWR);

  return 0;
}
