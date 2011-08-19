#ifndef ROSE_STR_H
#define ROSE_STR_H

#include "rmd.h"

typedef struct Str Str;

struct Str {
	char *start;
	uint32_t size;
};

void str_init(Str *str, char *start, const RMDHeader *h);

static inline void *str_addr(Str *str, uint32_t offset)
{
	return str->start + offset;
}

#endif

