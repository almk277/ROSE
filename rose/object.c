#include "object.h"
#include "module.h"
#include "heap.h"

#define object_field(obj, idx) *(R_Word*)((R_Word*)(obj + 1) + idx)

RD_Object *object_new(struct Module *module)
{
	RD_Object *o = (RD_Object*)heap_alloc(
			sizeof(RD_Object) + module_data_size(module));
	o->module = module;
	return o;
}

R_Word object_load(const RD_Object *obj, RA_Data idx)
{
	return object_field(obj, idx);
}

void object_store(RD_Object *obj, RA_Data idx, R_Word value)
{
	object_field(obj, idx) = value;
}

