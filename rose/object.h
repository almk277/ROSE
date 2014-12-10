#ifndef ROSE_OBJECT_H
#define ROSE_OBJECT_H

#include "heap.h"
struct Module;

typedef struct RD_Object {
	RefData refdata;
	struct Module *module;
} RD_Object;

#endif

