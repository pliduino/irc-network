#include <stdlib.h>

#include "../list/api.h"
#include "../monitor/api.h"

#include "api.h"

struct tlist
{
  monitor_t *monitor;
};

typedef struct insert_args insert_args_t;
struct insert_args
{
  uint32_t index;
  void *data;
};

typedef enum function_id
{
  GET,
  INSERT,
  PUSH,
  REMOVE,
  GET_SIZE,
  SET_ITEM_DATA_DESTROY,
} function_id_e;

static void *_monitor_get(void *shared_data, void *args)
{
  list_t *list = (list_t *)shared_data;
  uint32_t *index_buffer = (uint32_t *)args;
  uint32_t index = *index_buffer;
  free(index_buffer);
  return list_get(list, index);
}

static void *_monitor_insert(void *shared_data, void *args)
{
  list_t *list = (list_t *)shared_data;
  insert_args_t *insert_args = (insert_args_t *)args;
  list_insert(list, insert_args->index, insert_args->data);
  free(insert_args);
  return NULL;
}

static void *_monitor_push(void *shared_data, void *args)
{
  list_t *list = (list_t *)shared_data;
  void *data = args;
  list_push(list, data);
  return NULL;
}

static void *_monitor_remove(void *shared_data, void *args)
{
  list_t *list = (list_t *)shared_data;
  void *data = args;
  list_remove(list, data);
  return NULL;
}

static void *_monitor_get_size(void *shared_data, void *args)
{
  list_t *list = (list_t *)shared_data;
  uint32_t *size_buffer = (uint32_t *)malloc(sizeof(uint32_t));
  *size_buffer = list_get_size(list);
  return size_buffer;
}

static void *_monitor_set_item_data_destroy(void *shared_data, void *args)
{
  list_t *list = (list_t *)shared_data;
  void (*item_data_destroy)(void **) = (void (*)(void **))args;
  list_set_item_data_destroy(list, item_data_destroy);
  return NULL;
}

tlist_t *tlist_init(uint32_t max_size)
{
  tlist_t *tl = (tlist_t *)malloc(sizeof(tlist_t));
  list_t *list = list_init(max_size);
  tl->monitor = monitor_init();
  monitor_set_shared_data(tl->monitor, (void *)list, list_destroy);
  monitor_append_function(tl->monitor, _monitor_get);
  monitor_append_function(tl->monitor, _monitor_insert);
  monitor_append_function(tl->monitor, _monitor_push);
  monitor_append_function(tl->monitor, _monitor_remove);
  monitor_append_function(tl->monitor, _monitor_get_size);
  monitor_append_function(tl->monitor, _monitor_set_item_data_destroy);
  return tl;
}

void *tlist_get(tlist_t *tl, uint32_t index)
{
  uint32_t *index_buffer = (uint32_t *)malloc(sizeof(uint32_t));
  *index_buffer = index;
  return monitor_run(tl->monitor, GET, (void *)index_buffer);
}

void tlist_insert(tlist_t *tl, uint32_t index, void *data)
{
  insert_args_t *insert_args = (insert_args_t *)malloc(sizeof(insert_args_t));
  insert_args->data = data;
  insert_args->index = index;
  monitor_run(tl->monitor, INSERT, (void *)insert_args);
}

void tlist_push(tlist_t *tl, void *data)
{
  monitor_run(tl->monitor, PUSH, data);
}

void tlist_remove(tlist_t *tl, void *data)
{
  monitor_run(tl->monitor, REMOVE, data);
}

uint32_t tlist_get_size(tlist_t *tl)
{
  uint32_t *size_buffer = (uint32_t *)monitor_run(tl->monitor, GET_SIZE, NULL);
  uint32_t size = *size_buffer;
  free(size_buffer);
  return size;
}

void tlist_set_item_data_destroy(tlist_t *tl, void (*item_data_destroy)(void **))
{
  monitor_run(tl->monitor, SET_ITEM_DATA_DESTROY, (void *)item_data_destroy);
}

void tlist_destroy(void **tl_pointer)
{
  tlist_t *tl = (tlist_t *)*tl_pointer;
  monitor_destroy((void **)&tl->monitor);
  free(tl);
  *tl_pointer = NULL;
}