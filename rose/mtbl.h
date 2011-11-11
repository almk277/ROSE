#ifndef ROSE_MTBL_H
#define ROSE_MTBL_H

#include "rmd.h"

typedef struct Mtbl Mtbl;

struct Mtbl {
	RMDModule *start;
	uint8_t size;
};

#endif

