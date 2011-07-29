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

/* Checks if tbl of type Storage* has at least size bytes free,
 * enlarges tbl if not */
#define CHECK_SPACE(tbl, size) do { \
	if(!tbl->current || (ARRAY_LEN - tbl->current->len) < size) { \
		tbl->current = array_new(); \
		SIMPLEQ_INSERT_TAIL(&tbl->head, tbl->current, entry); \
	} \
} while(0)

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
	a->len += len;
}

uint16_t storage_add_str(Storage *tbl, const char *str)
{
	uint16_t addr = tbl->len;
	size_t len = strlen(str) + 1;
	CHECK_SPACE(tbl, len);
	array_add_str(tbl->current, str, len);
	tbl->len += len;
	return addr;
}

void storage_add_instr(Storage *tbl, uint8_t opcode, uint8_t oper)
{
	Array *a;
	CHECK_SPACE(tbl, 2);
	a = tbl->current;
	a->array[a->len++] = opcode;
	a->array[a->len++] = oper;
	tbl->len += 2;
}

uint32_t *storage_skip32(Storage *tbl)
{
	uint32_t *addr;
	Array *a;
	CHECK_SPACE(tbl, 4);
	a = tbl->current;
	addr = (uint32_t *)&a->array[a->len];
	a->len += 4;
	return addr;
}

void storage_add_byte(Storage *tbl, char byte)
{
	Array *a;
	CHECK_SPACE(tbl, 1);
	a = tbl->current;
	a->array[a->len++] = byte;
	++tbl->len;
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

