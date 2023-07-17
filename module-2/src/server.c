#include <stdlib.h>
#include <errno.h>

#include "../lib/errutils/api.h"
#include "../lib/ircd/api.h"

int main(int argc, char **argv)
{
  const uint16_t IRC_ALTERNATIVE_PORT = 6665;
  const uint16_t MESSAGE_MAX_SIZE = 4096;
  uint16_t port = IRC_ALTERNATIVE_PORT;

  if (argc > 2)
    errutils_exit(E2BIG, NULL, NULL);

  if (argc == 2)
  {
    const int DECIMAL_BASE = 10;
    errno = 0;
    port = (uint16_t)strtol(argv[1], NULL, DECIMAL_BASE);
    if (errno != 0)
      errutils_exit(0, NULL, NULL);
  }

  ircd_t *ircd_server = ircd_init();
  ircd_set_port(ircd_server, port);
  ircd_set_message_max_size(ircd_server, MESSAGE_MAX_SIZE);
  ircd_listen(ircd_server);
  ircd_destroy((void **)&ircd_server);

  return EXIT_SUCCESS;
}