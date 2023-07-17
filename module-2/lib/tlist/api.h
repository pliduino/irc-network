#ifndef _TLIST_API_H_
#define _TLIST_API_H_

#include <stdint.h>

typedef struct tlist tlist_t;

tlist_t *tlist_init(uint32_t max_size);
void *tlist_get(tlist_t *tl, uint32_t index);
void tlist_insert(tlist_t *tl, uint32_t index, void *data);
void tlist_push(tlist_t *tl, void *data);
void tlist_remove(tlist_t *tl, void *data);
uint32_t tlist_get_size(tlist_t *tl);
void tlist_set_item_data_destroy(tlist_t *tl, void (*item_data_destroy)(void **));
void tlist_destroy(void **tl_pointer);

#endif