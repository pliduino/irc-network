#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/poll.h>

#include "../errutils/api.h"
#include "../tqueue/api.h"
#include "../tlist/api.h"

#include "internal.h"
#include "api.h"

struct ircd
{
  uint16_t port;
  uint16_t message_max_size;
  uint16_t max_pending_connections;
  tlist_t *queue_list;
  ramclean_t *ramclean;
};

typedef struct client_connection client_connection_t;
struct client_connection
{
  struct sockaddr_in address;
  socklen_t socket_len;
  int32_t socket;
};

static void _exit_function(void *args)
{
  ircd_t *d = (ircd_t *)args;
  ircd_destroy((void **)&d);
}

static void _run_connection_thread(ircd_t *d, void *(*routine)(void *), int32_t client_socket, tqueue_t *queue, int nickname)
{
  pthread_t thread_id;
  pthread_attr_t thread_attr;

  connection_thread_args_t *buffer_args = malloc(sizeof(connection_thread_args_t));
  buffer_args->client_socket = client_socket;
  buffer_args->queue = queue;
  buffer_args->nickname = malloc(51);
  sprintf(buffer_args->nickname, "%d", nickname);
  buffer_args->tlist = d->queue_list;

  if (pthread_attr_init(&thread_attr))
    errutils_exit(0, _exit_function, (void *)d);

  if (pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED))
    errutils_exit(0, _exit_function, (void *)d);

  if (pthread_create(&thread_id, &thread_attr, routine, (void *)buffer_args))
    errutils_exit(0, _exit_function, (void *)d);
}

ircd_t *ircd_init()
{
  ircd_t *d = (ircd_t *)malloc(sizeof(ircd_t));
  d->port = 194;
  d->message_max_size = 4096;
  d->max_pending_connections = 8;
  d->ramclean = ramclean_init();
  d->queue_list = tlist_init(64);
  ramclean_append(d->ramclean, (void **)&d->queue_list, tlist_destroy);
  return d;
}

void ircd_set_port(ircd_t *d, uint16_t port)
{
  d->port = port;
}

void ircd_set_message_max_size(ircd_t *d, uint16_t message_max_size)
{
  d->message_max_size = message_max_size;
}

void ircd_listen(ircd_t *d)
{
  int id = 0;
  printf("Server is running!\n");

  int32_t socket_file_descriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (socket_file_descriptor < 0)
    errutils_exit(0, _exit_function, (void *)d);

  struct sockaddr_in main_socket_address;
  main_socket_address.sin_family = AF_INET;
  main_socket_address.sin_port = htons(d->port);
  main_socket_address.sin_addr.s_addr = htonl(INADDR_ANY);

  if (setsockopt(socket_file_descriptor, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
    errutils_exit(0, _exit_function, (void *)d);

  if (bind(socket_file_descriptor, (struct sockaddr *)&main_socket_address, sizeof(main_socket_address)) < 0)
    errutils_exit(0, _exit_function, (void *)d);

  listen(socket_file_descriptor, d->max_pending_connections);

  client_connection_t client_connection;
  client_connection.socket_len = sizeof(client_connection.address);

  while ((client_connection.socket = accept(socket_file_descriptor, (struct sockaddr *)&client_connection.address, (socklen_t *)&client_connection.socket_len)))
  {
    if (client_connection.socket == -1)
      errutils_exit(0, _exit_function, (void *)d);

    char *client_ip_addr = inet_ntoa(client_connection.address.sin_addr);
    printf("Connected to %s\n", client_ip_addr);

    tqueue_t *queue = tqueue_init();
    tlist_push(d->queue_list, (void *)queue);

    _run_connection_thread(d, ircd_connection_send, client_connection.socket, queue, id);
    _run_connection_thread(d, ircd_connection_receive, client_connection.socket, queue, id);
    id++;
  }

  shutdown(socket_file_descriptor, SHUT_RDWR);
}

void ircd_destroy(void **d_pointer)
{
  ircd_t *d = (ircd_t *)*d_pointer;
  ramclean_run(d->ramclean);
  ramclean_destroy((void **)&d->ramclean);
  free(d);
  *d_pointer = NULL;
}