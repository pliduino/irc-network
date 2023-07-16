#ifndef _IRCD_API_H_
#define _IRCD_API_H_

#include <stdint.h>

#include "../ramclean/api.h"

typedef struct ircd ircd_t;

ircd_t *ircd_init();
void ircd_set_port(ircd_t *d, uint16_t port);
void ircd_set_message_max_size(ircd_t *d, uint16_t message_max_size);
void ircd_listen(ircd_t *d);
void ircd_destroy(void **d_pointer);

#endif