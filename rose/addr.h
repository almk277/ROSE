#ifndef ROSE_ADDR_H
#define ROSE_ADDR_H

#include "rmd.h"

typedef struct Addr Addr;

struct Addr {
	uint32_t *start;
	uint8_t size;
};

void addr_init(Addr *addr, char *start, const RMDHeader *h);

static inline uint32_t addr_get(const Addr *addr, int idx)
{
	return addr->start[idx];
}

void addr_debug(const Addr *addr);

#endif

