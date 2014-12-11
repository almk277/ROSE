#include "array.h"
#include "heap.h"
#include "serial.h"
#include <stdlib.h>
#include <stdio.h>

#define CHECKED(a, idx, body) (idx < a->size ? ((body), 0) : -1)

/* RD_Array: addressing by words */
#define array_elem(a, idx) (*(R_Word*)((R_Word*)(a + 1) + idx))
/* RD_ByteArray: addressing by bytes */
#define array_byte(a, idx) (*(R_Byte*)((R_Byte*)(a + 1) + idx))
#define array_word(a, idx) (*(R_Word*)((R_Byte*)(a + 1) + idx))

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

int array_get(const RD_Array *a, RA_Array idx, R_Word *value)
{
	return CHECKED(a, idx, *value = array_word(a, idx));
}

int array_put(RD_Array *a, RA_Array idx, R_Word value)
{
	return CHECKED(a, idx, array_word(a, idx) = value);
}

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

RD_ByteArray *bytearray_load(const void *source)
{
	RA_Array size = deserial(*(RA_Array*)source);
	RD_ByteArray *a = bytearray_new(size);
	memcpy(a + 1, (RA_Array*)source + 1, size);
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

void bytearray_print(const RD_ByteArray *a)
{
	fwrite(a + 1, a->size, 1, stdout);
}

