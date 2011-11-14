#ifndef ROSE_DEBUG_H
#define ROSE_DEBUG_H

#include "compiler.h"
#include "segment.h"

C_ATTR_PRINTF(1, 2)
void debug(const char *fmt, ...);

void exp_dump(Exp *exp);

#endif

