#ifndef ROSE_ARRAY_H
#define ROSE_ARRAY_H

#include "rmd.h"

typedef struct RD_Array RD_Array;

typedef struct RD_ByteArray RD_ByteArray;

RD_Array *array_new(RA_Array size);

RA_Array array_length(const RD_Array *array);

int array_get(const RD_Array *a, RA_Array idx, R_Word *value);

int array_put(RD_Array *a, RA_Array idx, R_Word value);


RD_ByteArray *bytearray_new(RA_Array size);

RD_ByteArray *butearray_load(void *source);

RA_Array bytearray_length(const RD_ByteArray *array);

int bytearray_get_byte(const RD_ByteArray *a, RA_Array idx, R_Word *value);

int bytearray_get_word(const RD_ByteArray *a, RA_Array idx, R_Word *value);

int bytearray_put_byte(RD_ByteArray *a, RA_Array idx, R_Byte value);

int bytearray_put_word(RD_ByteArray *a, RA_Array idx, R_Word value);

#endif

