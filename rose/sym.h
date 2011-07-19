#ifndef ROSE_SYM_H
#define ROSE_SYM_H

#include "rmd.h"

typedef struct Sym Sym;

struct Sym {
	const char *start;
	uint16_t size;
};

void sym_init(Sym *sym, const char *start, const RMDHeader *h);

static inline const char *sym_get(const Sym *sym, uint16_t ofs)
{
	return &sym->start[ofs];
}

#endif

