#ifndef _IRCD_INTERNAL_H_
#define _IRCD_INTERNAL_H_

#include "../monitor/api.h"
#include "../tqueue/api.h"
#include "../tlist/api.h"

typedef struct channel_queue channel_queue_t;
struct channel_queue
{
  char *channel;
  tqueue_t *queue;
};

typedef struct connection_thread_args connection_thread_args_t;
struct connection_thread_args
{
  int32_t client_socket;
  channel_queue_t *channel_queue;
  tlist_t *queue_list;
  tlist_t *nick_list;
  tlist_t *channel_list;
  char *nickname;
};

uint8_t channel_name_is_valid(char *channel_name);
void *ircd_connection_send(void *args);
void *ircd_connection_receive(void *args);

#endif
