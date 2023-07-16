#ifndef _MONITOR_API_H_
#define _MONITOR_API_H_

#include <pthread.h>
#include <stdint.h>

typedef struct monitor monitor_t;

monitor_t *monitor_init();
uint8_t monitor_append_function(monitor_t *m, void *(*function)(void *, void *));
void monitor_set_shared_data(monitor_t *m, void **shared_data, void (*shared_data_destroy)(void **));
void *monitor_run(monitor_t *m, int function_id, void *args);
void monitor_destroy(void **m_pointer);

#endif