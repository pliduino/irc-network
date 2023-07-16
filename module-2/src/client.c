#include "../lib/ircclient/api.h"

#include <arpa/inet.h>
#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <sys/socket.h>

#include <unistd.h>
#define PORT 6665
#define BUFFER_SIZE 4096

int main()
{
  struct sockaddr_in serv_address = {0};

  int sockfd;
  if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
  {
    printf("Error while creating the socket\n");
    return -1;
  }

  serv_address.sin_family = AF_INET;
  serv_address.sin_port = htons(PORT);

  if (inet_pton(AF_INET, "127.0.0.1", &serv_address.sin_addr) <= 0)
  {
    printf("Invalid address");
    return -1;
  }

  printf("**\tConnecting to Server\t**\n");
  int status =
      connect(sockfd, (struct sockaddr *)&serv_address, sizeof(serv_address));

  if (status < 0)
  {
    printf("Connection Failed");
    return -1;
  }

  pthread_t thread_id[2];

  pthread_create(&thread_id[0], NULL, ircclient_connection_receive, (void *)&sockfd);

  pthread_create(&thread_id[1], NULL, ircclient_connection_send, (void *)&sockfd);

  pthread_join(thread_id[0], NULL);
  pthread_join(thread_id[1], NULL);

  printf("**\tClosing Connection\t**\n");
  close(sockfd);

  return 0;
}
