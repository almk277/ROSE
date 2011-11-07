#ifndef ROSE_ARRAY_H
#define ROSE_ARRAY_H

#include "object.h"
#include "error.h"
#include "ref.h"
#include <stdint.h>
#include <stddef.h>

typedef struct Array Array;

struct Array {
	Object __object;
	uint32_t size;
	int32_t data[1];
};

Ref array_new(uint32_t count);

void array_delete(Ref array);

#define array_check_idx(array, idx) \
	do { \
		if(idx >= array->size) \
			error(ERR_OUT_OF_BAND); \
	} while(0)

int32_t array_get(Ref array, uint32_t idx);

void array_put(Ref array, uint32_t idx, int32_t data);

uint32_t array_len(Ref array);

#endif

