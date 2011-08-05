#ifndef ROSE_ADDRTBL_H
#define ROSE_ADDRTBL_H

#include "conf.h"
#include <stdint.h>

typedef struct AddrTbl AddrTbl;
typedef void *Pointer;

struct AddrTbl {
	Pointer tbl[ADDRTBL_SIZE];
	uint32_t top;
};

extern AddrTbl addrtbl_first;

static inline Pointer addrtbl_get(const AddrTbl *tbl, uint32_t idx)
{
	return tbl->tbl[idx];
}

static inline Pointer *addrtbl_addr(AddrTbl *tbl, uint32_t idx)
{
	return &tbl->tbl[idx];
}

static inline int addrtbl_is_full(const AddrTbl *tbl)
{
	return tbl->top == ADDRTBL_SIZE - 1;
}

static inline uint32_t addrtbl_top(AddrTbl *tbl)
{
	return tbl->top;
}

AddrTbl *addrtbl_new(void);

void addrtbl_delete(AddrTbl *tbl);

uint32_t addrtbl_add_ptr(AddrTbl *tbl, Pointer ptr);

#endif

