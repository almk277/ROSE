#ifndef ROSE_PTBL_H
#define ROSE_PTBL_H

#include "rmd.h"
#include <stddef.h>

typedef struct Ptbl Ptbl;

struct Ptbl {
	RMDProcedure *start;
	uint8_t size;
};

void ptbl_init(Ptbl *ptbl, char *start, const RMDHeader *h);

static inline RMDProcedure *ptbl_get(const Ptbl *ptbl, int idx)
{
	return &ptbl->start[idx];
}

#endif

