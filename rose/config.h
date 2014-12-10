#ifndef ROSE_CONFIG_H
#define ROSE_CONFIG_H

#include <stddef.h>

#define ROSE_LARGE (sizeof(void*) > sizeof(R_Word))

size_t heap_size(void);

#endif

