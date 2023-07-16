#include "server.h"
#include "connection.h"

#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <sys/socket.h>

#include <unistd.h>
#define PORT 8080
#define BUFFER_SIZE 4096

int server_execute() {
  struct sockaddr_in own_address, client_address;
  int sockfd;
  int new_socket;

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

  printf("**\tWaiting for Client\t**\n");

  new_socket =
      accept(sockfd, (struct sockaddr *)&client_address, (socklen_t *)&addrlen);

  printf("**\tConnected to Client\t**\n");

  pthread_t thread_id[2];

  pthread_create(&thread_id[0], NULL, receive_thread, (void *)&new_socket);

  pthread_create(&thread_id[1], NULL, send_thread, (void *)&new_socket);

  pthread_join(thread_id[0], NULL);
  pthread_join(thread_id[1], NULL);

  printf("**\tClosing Connection\t**\n");
  close(new_socket);

  printf("**\tShutting Down Server\t**\n");
  shutdown(sockfd, SHUT_RDWR);

  return 0;
}
