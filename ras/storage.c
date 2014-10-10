#include "storage.h"
#include "print.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

extern FILE *output;
extern int verbose;

struct Slice {
	SIMPLEQ_ENTRY(Slice) entry;
	uint32_t len;
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

void storage_enlarge(Storage *tbl, uint32_t size)
{
	if(!tbl->current || (SLICE_LEN - tbl->current->len) < size) {
		tbl->current = array_new();
		SIMPLEQ_INSERT_TAIL(&tbl->head, tbl->current, entry);
	}
}

/* copies size bytes from addr to s */
/*
void storage_copy(Storage *s, const void *addr, uint32_t size)
{
	Slice *a = s->current;
	memcpy(&a->array[a->len], addr, size);
	a->len += size;
	s->len += size;
}
*/

void storage_put1byte(Storage *s, char byte)
{
	Slice *a = s->current;
	a->array[a->len] = byte;
	a->len += 1;
	s->len += 1;
}

void storage_put2byte(Storage *s, int16_t word)
{
	Slice *a = s->current;
	*(int16_t*)&a->array[a->len] = word;
	a->len += 2;
	s->len += 2;
}

void storage_put4byte(Storage *s, int32_t word)
{
	Slice *a = s->current;
	*(int32_t*)&a->array[a->len] = word;
	a->len += 4;
	s->len += 4;
}

uint32_t storage_add_string(Storage *tbl, const String *str)
{
	uint32_t cur = tbl->len;
	Slice *s;
	const int len = str->len;

	storage_enlarge(tbl, len);
	s = tbl->current;
	memcpy(&s->array[s->len], str->data, len);
	s->len += len;
	tbl->len += len;

	return cur;
}

#ifdef DEBUG
static void strarray_print(const Slice *a)
{
	uint32_t i;
	for(i = 0; i != a->len; ++i) {
		int ch = a->array[i];
		putchar(ch ? ch : '~');
	}
}

void storage_print_str(const Storage *tbl)
{
	Slice *a;
	SIMPLEQ_FOREACH(a, &tbl->head, entry) {
		if(verbose >= DL_NUDE)
			printf("\n (%d)- ", a->len);
		strarray_print(a);
	}
	putchar('\n');
}
#endif

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
static uint32_t *array_current_len;

uint32_t array_begin(Storage *tbl)
{
	Slice *s;
	uint32_t current_addr;

	array_current_storage = tbl;
	storage_enlarge(tbl, sizeof *array_current_len);
	s = tbl->current;
	current_addr = tbl->len;
	array_current_len = (uint32_t*)&s->array[s->len];
	s->len += sizeof *array_current_len;
	tbl->len += sizeof *array_current_len;

	return current_addr;
}

void array_add_byte(char byte)
{
	storage_enlarge(array_current_storage, 1);
	storage_put1byte(array_current_storage, byte);
	++*array_current_len;
}

void array_add_string(const String *string)
{
	storage_add_string(array_current_storage, string);
	*array_current_len += string->len;
}

