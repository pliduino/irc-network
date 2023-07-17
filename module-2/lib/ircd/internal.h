#ifndef _IRCD_INTERNAL_H_
#define _IRCD_INTERNAL_H_

#include "../monitor/api.h"
#include "../tqueue/api.h"
#include "../tlist/api.h"

typedef struct connection_thread_args connection_thread_args_t;
struct connection_thread_args
{
  int32_t client_socket;
  tqueue_t *queue;
  tlist_t *tlist;
  char *nickname;
};

void *ircd_connection_send(void *args);
void *ircd_connection_receive(void *args);

#endif
