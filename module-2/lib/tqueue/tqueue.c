#include <stdlib.h>
#include <stdint.h>

#include "../queue/api.h"
#include "../monitor/api.h"

#include "api.h"

struct tqueue
{
  monitor_t *monitor;
};

typedef enum function_id
{
  PUSH,
  POP,
  SET_ITEM_DATA_DESTROY
} function_id_e;

static void *_monitor_push(void *shared_data, void *args)
{
  queue_t *queue = (queue_t *)shared_data;
  void **data_pointer = (void **)args;
  queue_push(queue, data_pointer);
  return NULL;
}

static void *_monitor_pop(void *shared_data, void *args)
{
  queue_t *queue = (queue_t *)shared_data;
  return queue_pop(queue);
}

static void *_monitor_set_item_data_destroy(void *shared_data, void *args)
{
  queue_t *queue = (queue_t *)shared_data;
  void (*item_data_destroy)(void **) = (void (*)(void **))args;
  queue_set_item_data_destroy(queue, item_data_destroy);
  return NULL;
}

tqueue_t *tqueue_init()
{
  tqueue_t *tq = (tqueue_t *)malloc(sizeof(tqueue_t));
  queue_t *queue = queue_init();
  tq->monitor = monitor_init();
  monitor_set_shared_data(tq->monitor, (void **)&queue, queue_destroy);
  monitor_append_function(tq->monitor, _monitor_push);
  monitor_append_function(tq->monitor, _monitor_pop);
  monitor_append_function(tq->monitor, _monitor_set_item_data_destroy);

  return tq;
}

void tqueue_push(tqueue_t *tq, void **data_pointer)
{
  monitor_run(tq->monitor, PUSH, (void *)data_pointer);
}

void *tqueue_pop(tqueue_t *tq)
{
  return monitor_run(tq->monitor, POP, NULL);
}

void tqueue_set_item_data_destroy(tqueue_t *tq, void (*item_data_destroy)(void **))
{
  monitor_run(tq->monitor, SET_ITEM_DATA_DESTROY, (void *)item_data_destroy);
}

void tqueue_destroy(void **tq_pointer)
{
  tqueue_t *tq = (tqueue_t *)*tq_pointer;
  monitor_destroy((void **)&tq->monitor);
  free(tq);
  *tq_pointer = NULL;
}