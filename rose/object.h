#ifndef ROSE_OBJECT_H
#define ROSE_OBJECT_H

#include "heap.h"
struct Module;

typedef struct RD_Object {
	RefData refdata;
	struct Module *module;
} RD_Object;

RD_Object *object_new(struct Module *module);

R_Word object_load(const RD_Object *obj, RA_Data idx);

void object_store(RD_Object *obj, RA_Data idx, R_Word value);

#endif

