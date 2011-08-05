#ifndef ROSE_STR_H
#define ROSE_STR_H

#include "rmd.h"

typedef struct Str Str;

struct Str {
	const char *start;
	uint32_t size;
};

void str_init(Str *str, const char *start, const RMDHeader *h);

#endif

