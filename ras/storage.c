#include "storage.h"
#include "common.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct Array {
	SIMPLEQ_ENTRY(Array) entry;
	size_t len;
	char array[ARRAY_LEN];
};

static Array *array_new(void)
{
	Array *a = malloc(sizeof *a);
	if(!a)
		error("out of memory");
	a->len = 0;
	return a;
}

static void array_add_str(Array *a, const char *str, size_t len)
{
	strcpy(&a->array[a->len], str);
	a->len += (len + 1);
}

uint16_t storage_add_str(Storage *tbl, const char *str)
{
	uint16_t addr = tbl->len;
	size_t len = strlen(str);
	Array *a = tbl->current;
	if(!a || (ARRAY_LEN - a->len) <= len) {
		tbl->current = a = array_new();
		SIMPLEQ_INSERT_TAIL(&tbl->head, a, entry);
	}
	array_add_str(a, str, len);
	tbl->len += (len + 1);
	return addr;
}

static void strarray_print(const Array *a)
{
	size_t i;
	for(i = 0; i != a->len; ++i) {
		int ch = a->array[i];
		putchar(ch ? ch : '~');
	}
}

void storage_print_str(const Storage *tbl)
{
	Array *a;
	SIMPLEQ_FOREACH(a, &tbl->head, entry) {
		if(verbose >= DL_NUDE)
			printf("\n (%d)- ", a->len);
		strarray_print(a);
	}
	putchar('\n');
}

void storage_add_instr(Storage *tbl, uint8_t opcode, uint8_t oper)
{
	Array *a = tbl->current;
	if(!a || a->len == ARRAY_LEN / 2) {
		tbl->current = a = array_new();
		SIMPLEQ_INSERT_TAIL(&tbl->head, a, entry);
	}
	a->array[a->len++] = opcode;
	a->array[a->len++] = oper;
	tbl->len += 2;
}

char *storage_current(const Storage *tbl)
{
	Array *a = tbl->current;
	return &a->array[a->len];
}

void storage_write(const Storage *tbl, FILE *file)
{
	const struct Array *a;
	SIMPLEQ_FOREACH(a, &tbl->head, entry)
		if(fwrite(a->array, a->len, 1, file) != 1)
			file_write_error();
}

