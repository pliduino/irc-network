#include "client.h"
#include "server.h"
#include <stdio.h>

#define PORT 8080

int main(int argc, char const *argv[]) {
  char input;

  printf("Which: s/c\n");

  scanf("%c", &input);

  if (input == 's') {
    server_execute();
  } else if (input == 'c') {
    client_execute();
  }

  return 0;
}
