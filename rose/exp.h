#ifndef ROSE_EXP_H
#define ROSE_EXP_H

#include "rmd.h"

typedef struct Exp Exp;

struct Exp {
	RMDExport *start;
	uint8_t size;
};

void exp_init(Exp *exp, char *start, const RMDHeader *h);

static inline RMDExport *exp_get(const Exp *exp, int idx)
{
	return &exp->start[idx];
}

static inline uint8_t exp_idx_get(const Exp *exp, int idx)
{
	return exp_get(exp, idx)->idx;
}

#endif

