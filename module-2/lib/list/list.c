#include <stdlib.h>

#include "api.h"

struct list
{
  void **items;
  uint32_t size;
  uint32_t max_size;
  void (*item_data_destroy)(void **);
};

list_t *list_init(uint32_t max_size)
{
  list_t *l = (list_t *)malloc(sizeof(list_t));
  l->items = (void **)malloc(max_size * sizeof(void *));
  l->size = 0;
  l->max_size = max_size;
  l->item_data_destroy = NULL;
  return l;
}

void *list_get(list_t *l, uint32_t index)
{
  return l->items[index];
}

void list_insert(list_t *l, uint32_t index, void *data)
{
  l->items[index] = data;
  l->size++;
}

void list_push(list_t *l, void *data)
{
  l->items[l->size] = data;
  l->size++;
}

void list_remove(list_t *l, void *data)
{
  for (uint32_t i = 0; i < l->size; i++)
    if (l->items[i] == data)
    {
      l->size--;
      for (uint32_t j = i; j < l->size; i++)
        l->items[j] = l->items[j + 1];
      return;
    }
}

uint32_t list_get_size(list_t *l)
{
  return l->size;
}

void list_set_item_data_destroy(list_t *l, void (*item_data_destroy)(void **))
{
  l->item_data_destroy = item_data_destroy;
}

void list_destroy(void **l_pointer)
{
  list_t *l = (list_t *)*l_pointer;

  if (l->item_data_destroy != NULL)
    for (uint32_t i = 0; i < l->size; i++)
      l->item_data_destroy(&l->items[i]);

  free(l->items);
  free(l);
  *l_pointer = NULL;
}