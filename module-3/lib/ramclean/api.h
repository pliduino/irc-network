#ifndef _RAMCLEAN_API_H_
#define _RAMCLEAN_API_H_

typedef struct ramclean ramclean_t;

ramclean_t *ramclean_init();
void ramclean_append(ramclean_t *t, void **struct_pointer, void (*destroy_function)(void **));
void ramclean_run(ramclean_t *t);
void ramclean_destroy(void **t_pointer);

#endif