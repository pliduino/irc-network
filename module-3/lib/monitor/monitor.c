#include <stdlib.h>

#include "api.h"

struct monitor
{
  pthread_mutex_t *mutex;
  uint8_t n_functions;
  void *(**functions)(void *, void *);
  void *shared_data;
  void (*shared_data_destroy)(void **);
};

monitor_t *monitor_init()
{
  const uint16_t UINT8_SIZE = 256;

  monitor_t *m = malloc(sizeof(monitor_t));
  m->n_functions = 0;
  m->shared_data = NULL;
  m->functions = malloc(UINT8_SIZE * sizeof(void *(*)(void *, void *)));
  m->mutex = malloc(sizeof(pthread_mutex_t));
  pthread_mutex_init(m->mutex, NULL);
  return m;
}

uint8_t monitor_append_function(monitor_t *m, void *(*function)(void *, void *))
{
  m->functions[m->n_functions] = function;
  return m->n_functions++;
}

void monitor_set_shared_data(monitor_t *m, void *shared_data, void (*shared_data_destroy)(void **))
{
  m->shared_data = shared_data;
  m->shared_data_destroy = shared_data_destroy;
}

void *monitor_run(monitor_t *m, int function_id, void *args)
{
  pthread_mutex_lock(m->mutex);
  void *r = m->functions[function_id](m->shared_data, args);
  pthread_mutex_unlock(m->mutex);
  return r;
}

void monitor_destroy(void **m_pointer)
{
  monitor_t *m = (monitor_t *)*m_pointer;

  if (m->shared_data_destroy != NULL)
    m->shared_data_destroy(&m->shared_data);

  free(m->functions);
  pthread_mutex_destroy(m->mutex);
  free(m->mutex);
  free(m);
  *m_pointer = NULL;
}