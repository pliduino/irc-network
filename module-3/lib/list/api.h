#ifndef _LIST_API_H_
#define _LIST_API_H_

#include <stdint.h>

typedef struct list list_t;

list_t *list_init(uint32_t max_size);
void *list_get(list_t *l, uint32_t index);
void list_insert(list_t *l, uint32_t index, void *data);
void list_push(list_t *l, void *data);
void list_remove(list_t *l, void *data);
uint32_t list_get_size(list_t *l);
void list_set_item_data_destroy(list_t *l, void (*item_data_destroy)(void **));
void list_destroy(void **l_pointer);

#endif