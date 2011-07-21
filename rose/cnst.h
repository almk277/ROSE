#ifndef ROSE_CNST_H
#define ROSE_CNST_H

#include "rmd.h"

typedef struct Cnst Cnst;

struct Cnst {
	int32_t *start;
	uint8_t size;
};

void cnst_init(Cnst *cnst, char *start, const RMDHeader *h);

static inline int32_t cnst_get(const Cnst *cnst, int idx)
{
	return cnst->start[idx];
}

void cnst_debug(Cnst *cnst);

#endif

