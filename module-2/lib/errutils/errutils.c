#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#include "api.h"

void errutils_exit(uint32_t errno_override, void (*exit_function)(void *), void *exit_function_args)
{
  if (errno_override != 0)
    errno = errno_override;

  if (exit_function != NULL)
    exit_function(exit_function_args);

  perror("Error");
  exit(EXIT_FAILURE);
}