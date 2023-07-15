#include "client.h"

#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <sys/socket.h>

#include <unistd.h>
#define PORT 8080

int client_execute() {
  struct sockaddr_in serv_address = {0};

  int sockfd;
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("Error while creating the socket\n");
    return -1;
  }

  serv_address.sin_family = AF_INET;
  serv_address.sin_port = htons(PORT);

  if (inet_pton(AF_INET, "127.0.0.1", &serv_address.sin_addr) <= 0) {
    printf("Invalid address");
    return -1;
  }

  printf("**\tConnecting to Server\t**\n");
  int status =
      connect(sockfd, (struct sockaddr *)&serv_address, sizeof(serv_address));

  if (status < 0) {
    printf("Connection Failed");
    return -1;
  }

  printf("**\tSending Message\t\t**\n");
  send(sockfd, "Hello", 5, 0);

  char buffer[1024] = {0};

  int bytes_received;

  printf("**\tReceiving Message\t**\n");
  if ((bytes_received = read(sockfd, buffer, 1024)) > 0) {
    printf("%d, %s\n", bytes_received, buffer);
  } else {
    printf("Message too small");
  }

  printf("**\tClosing Connection\t**\n");
  close(sockfd);

  return 0;
}
