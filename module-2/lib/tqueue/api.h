#ifndef _TQUEUE_API_H_
#define _TQUEUE_API_H_

typedef struct tqueue tqueue_t;

tqueue_t *tqueue_init();
void tqueue_push(tqueue_t *tq, void **data_pointer);
void *tqueue_pop(tqueue_t *tq);
void tqueue_set_item_data_destroy(tqueue_t *tq, void (*item_data_destroy)(void **));
void tqueue_destroy(void **tq_pointer);

#endif