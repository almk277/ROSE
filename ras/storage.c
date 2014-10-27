#include "storage.h"
#include "symbol.h"
#include "print.h"
#include "endian.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

/* one array length */
#define SLICE_LEN  (2 * 1024)

extern FILE *output;
extern int verbose;

struct Slice {
	SIMPLEQ_ENTRY(Slice) entry;
	RA_Array len;
	char array[SLICE_LEN];
};

static Slice *array_new()
{
	Slice *a = malloc(sizeof *a);
	if(!a)
		error("out of memory");
	a->len = 0;
	return a;
}

void storage_enlarge(Storage *tbl, RA_Array size)
{
	if(!tbl->current || (SLICE_LEN - tbl->current->len) < size) {
		tbl->current = array_new();
		SIMPLEQ_INSERT_TAIL(&tbl->head, tbl->current, entry);
	}
}

void storage_put1byte(Storage *s, uint8_t byte)
{
	Slice *a = s->current;
	a->array[a->len] = byte;
	a->len += 1;
	s->len += 1;
}

void storage_put2byte(Storage *s, int16_t word)
{
	Slice *a = s->current;
	*(int16_t*)&a->array[a->len] = serial_16(word);
	a->len += 2;
	s->len += 2;
}

void storage_put4byte(Storage *s, int32_t word)
{
	Slice *a = s->current;
	*(int32_t*)&a->array[a->len] = serial_32(word);
	a->len += 4;
	s->len += 4;
}

RA_Array storage_add_symbol(Storage *tbl, const Symbol *sym)
{
	RA_Array cur = tbl->len;
	Slice *s;
	const int len = symbol_store_length(sym);

	storage_enlarge(tbl, len);
	s = tbl->current;
	symbol_copy_to(sym, &s->array[s->len]);
	s->len += len;
	tbl->len += len;

	return cur;
}

char *storage_current(const Storage *tbl)
{
	Slice *a = tbl->current;
	return &a->array[a->len];
}

void storage_write(const Storage *tbl)
{
	const struct Slice *a;
	SIMPLEQ_FOREACH(a, &tbl->head, entry)
		if(fwrite(a->array, a->len, 1, output) != 1)
			file_write_error();
}

static Storage *array_current_storage;
static RA_Array current_len;
static RA_Array *current_len_addr;

RA_Array array_begin(Storage *tbl)
{
	Slice *s;
	RA_Array current_addr;

	array_current_storage = tbl;
	storage_enlarge(tbl, sizeof(RA_Array));
	s = tbl->current;
	current_addr = tbl->len;
	current_len_addr = (RA_Array*)&s->array[s->len];
	current_len = 0;
	s->len += sizeof(RA_Array);
	tbl->len += sizeof(RA_Array);

	return current_addr;
}

void array_add_byte(char byte)
{
	storage_enlarge(array_current_storage, 1);
	storage_put1byte(array_current_storage, byte);
	++current_len_addr;
}

void array_end()
{
	*current_len_addr = serial_32(current_len);
}

