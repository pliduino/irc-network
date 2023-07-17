#ifndef _ERRUTILS_API_H_
#define _ERRUTILS_API_H_

#include <stdint.h>

void errutils_exit(uint32_t errno_override, void (*exit_function)(void *), void *exit_function_args);

#endif