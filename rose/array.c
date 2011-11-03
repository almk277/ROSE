#include "array.h"
#include <stdlib.h>

Ref array_new(size_t count)
{
	Array *a = malloc(sizeof(Array) + count);
	check_ptr(a);
	a->size = count;
	object_init(&a->__object);
	return ref_new(a);
}

void array_delete(Ref array)
{
	Array *a = ref_get(array);
	ref_delete(array);
	object_delete(&a->__object);
	free(a);
}

int32_t array_get(Ref array, size_t idx)
{
	Array *a = ref_get(array);
	array_check_idx(a, idx);
	return a->data[idx];
}

void array_put(Ref array, size_t idx, int32_t data)
{
	Array *a = ref_get(array);
	array_check_idx(a, idx);
	a->data[idx] = data;
}

