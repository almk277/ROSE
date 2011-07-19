#ifndef ROSE_DEBUG_H
#define ROSE_DEBUG_H

#include "compiler.h"

C_ATTR_PRINTF(1, 2)
void debug(const char *fmt, ...);

#endif

