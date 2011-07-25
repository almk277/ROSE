#ifndef ROSE_ADDRCAT_H
#define ROSE_ADDRCAT_H

#include "conf.h"
#include "addrtbl.h"
#include <stdint.h>

typedef struct AddrCat AddrCat;

struct AddrCat {
	AddrTbl *cat[ADDRCAT_SIZE];
	uint32_t top;
};

static inline AddrTbl *addrcat_get(const AddrCat *cat, uint32_t idx)
{
	return cat->cat[idx];
}

static inline uint32_t addrcat_top(const AddrCat *cat)
{
	return cat->top;
}

static inline int addrcat_is_full(const AddrCat *cat)
{
	return cat->top == ADDRCAT_SIZE - 1;
}

void addrcat_add_tbl(AddrCat *cat, AddrTbl *tbl);

#endif

