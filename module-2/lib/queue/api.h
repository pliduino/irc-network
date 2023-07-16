#ifndef _QUEUE_API_H_
#define _QUEUE_API_H_

typedef struct queue queue_t;

queue_t *queue_init();
void queue_push(queue_t *q, void **data_pointer);
void queue_set_item_data_destroy(queue_t *q, void (*item_data_destroy)(void **));
void *queue_pop(queue_t *q);
void queue_destroy(void **q_pointer);

#endif