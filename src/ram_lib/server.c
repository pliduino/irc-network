#include "server.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <sys/socket.h>

#include <unistd.h>
#define PORT 8080
int server_execute()
{
    struct sockaddr_in own_address = {0};

    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Error while creating the socket\n");
        exit(1);
    }

    own_address.sin_family = AF_INET;
    own_address.sin_port = htons(PORT);
    own_address.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (struct sockaddr *)&own_address, sizeof(own_address)) < 0)
    {
        printf("Error in binding\n");
        exit(1);
    }

    listen(sockfd, 3);

    int addrlen;
    struct sockaddr client_address;

    accept(sockfd, &client_address, &addrlen);

    return 0;
}