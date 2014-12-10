#include "array.h"
#include "heap.h"
#include <stdlib.h>

#define CHECKED(a, idx, body) (idx <= a->size ? -1 : (body), 0)

#define array_byte(a, idx) (*(R_Byte*)((char*)a + sizeof(RD_ByteArray) + idx))
#define array_word(a, idx) (*(R_Word*)((char*)a + sizeof(RD_ByteArray) + idx))

struct RD_Array {
	RefData refdata;
	RA_Array size;
};

RD_Array *array_new(RA_Array size)
{
	RD_Array *a = (RD_Array*)heap_alloc(sizeof(RD_Array) + size);
	a->size = size;
	return a;
}

RA_Array array_length(const RD_Array *array)
{
	return array->size;
}

int array_get(const RD_Array *a, RA_Array idx, R_Word *value);

int array_put(RD_Array *a, RA_Array idx, R_Word value);


struct RD_ByteArray {
	RefData refdata;
	RA_Array size;
};

RD_ByteArray *bytearray_new(RA_Array size)
{
	RD_ByteArray *a = (RD_ByteArray*)heap_alloc(sizeof(RD_ByteArray) + size);
	a->size = size;
	return a;
}

RD_ByteArray *butearray_load(void *source)
{
	RA_Array size = *(RA_Array*)source;
	RD_ByteArray *a = bytearray_new(size);
	memcpy((char*)a + sizeof(RD_ByteArray), (RA_Array*)source + 1, size);
	return a;
}

RA_Array bytearray_length(const RD_ByteArray *array)
{
	return array->size;
}

int bytearray_get_byte(const RD_ByteArray *a, RA_Array idx, R_Word *value)
{
	return CHECKED(a, idx, *value = array_byte(a, idx));
}

int bytearray_get_word(const RD_ByteArray *a, RA_Array idx, R_Word *value)
{
	return CHECKED(a, idx, *value = array_word(a, idx));
}

int bytearray_put_byte(RD_ByteArray *a, RA_Array idx, R_Byte value)
{
	return CHECKED(a, idx, array_byte(a, idx) = value);
}

int bytearray_put_word(RD_ByteArray *a, RA_Array idx, R_Word value)
{
	return CHECKED(a, idx, array_word(a, idx) = value);
}

