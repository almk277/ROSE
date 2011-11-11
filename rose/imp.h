#ifndef ROSE_IMP_H
#define ROSE_IMP_H

#include "rmd.h"

typedef struct Imp Imp;

struct Imp {
	RMDImport *start;
	uint8_t size;
};

#endif

