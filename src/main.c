#include "client.h"
#include "server.h"
#include <stdio.h>
#include <winsock2.h>

#define PORT 8080

int main(int argc, char const *argv[])
{
    char input;

    printf("Which: S/C\n");

    scanf("%c", &input);

    if (input == 'S')
    {
        server_execute();
    }
    else if (input == 'C')
    {
        client_execute();
    }

    return 0;
}
