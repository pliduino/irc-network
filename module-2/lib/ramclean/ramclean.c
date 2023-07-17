#include <stdlib.h>
#include <stdint.h>

#include "api.h"

struct ramclean
{
  void ***struct_pointers;
  void (**destroy_functions)(void **);
  uint8_t n_pointers;
};

ramclean_t *ramclean_init()
{
  const uint16_t UINT8_SIZE = 256;

  ramclean_t *t = malloc(sizeof(ramclean_t));
  t->n_pointers = 0;
  t->struct_pointers = malloc(UINT8_SIZE * sizeof(void **));
  t->destroy_functions = malloc(UINT8_SIZE * sizeof(void *(*)(void **)));
  return t;
}

void ramclean_append(ramclean_t *t, void **struct_pointer, void (*destroy_function)(void **))
{
  if (struct_pointer == NULL || destroy_function == NULL)
    return;
  t->struct_pointers[t->n_pointers] = struct_pointer;
  t->destroy_functions[t->n_pointers] = destroy_function;
  t->n_pointers++;
}

void ramclean_run(ramclean_t *t)
{
  for (int i = 0; i < t->n_pointers; i++)
    if (*t->struct_pointers[i] != NULL)
      t->destroy_functions[i](t->struct_pointers[i]);
  t->n_pointers = 0;
}

void ramclean_destroy(void **t_pointer)
{
  ramclean_t *t = (ramclean_t *)*t_pointer;
  free(t->struct_pointers);
  free(t->destroy_functions);
  free(t);
  *t_pointer = NULL;
}