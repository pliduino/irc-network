#include <stdlib.h>

#include "api.h"

typedef struct queue_item queue_item_t;
struct queue_item
{
  void *data;
  queue_item_t *prev;
};

struct queue
{
  queue_item_t *head;
  queue_item_t *tail;
  void (*item_data_destroy)(void **);
};

queue_t *queue_init()
{
  queue_t *q = (queue_t *)malloc(sizeof(queue_t));
  q->head = NULL;
  q->tail = NULL;
  q->item_data_destroy = NULL;
  return q;
}

void queue_set_item_data_destroy(queue_t *q, void (*item_data_destroy)(void **))
{
  q->item_data_destroy = item_data_destroy;
}

void queue_push(queue_t *q, void *data)
{
  if (data == NULL)
    return;

  queue_item_t *item = (queue_item_t *)malloc(sizeof(queue_item_t));
  item->data = data;
  item->prev = NULL;

  if (q->tail != NULL)
    q->tail->prev = item;
  q->tail = item;

  if (q->head == NULL)
    q->head = item;
}

void *queue_pop(queue_t *q)
{
  if (q->head == NULL)
    return NULL;

  queue_item_t *item = q->head;

  if (q->head == q->tail)
    q->tail = NULL;

  q->head = q->head->prev;

  void *data = item->data;
  free(item);

  return data;
}

void queue_destroy(void **q_pointer)
{
  queue_t *q = (queue_t *)*q_pointer;

  while (1)
  {
    void *head_data = queue_pop(q);
    if (head_data == NULL)
      break;
    if (q->item_data_destroy != NULL)
      q->item_data_destroy(&head_data);
  }

  free(q);
  *q_pointer = NULL;
}