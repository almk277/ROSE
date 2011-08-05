/* storage is list of byte arrays.
 * It grows dynamically when the size is not enough */

#ifndef STORAGE_H
#define STORAGE_H

#include "queue.h"
#include <stdint.h>
#include <stdio.h>

/* one array length */
#define ARRAY_LEN  (2 * 1024)

typedef struct Array Array;

SIMPLEQ_HEAD(QueueHead, Array);

/* type descriptor, must be initialized */
typedef struct Storage {
	struct QueueHead head;
	Array *current;
	size_t len;
} Storage;

/* initializer for Storage */
#define STORAGE_INITIALIZER(table) \
	{ SIMPLEQ_HEAD_INITIALIZER(table.head), 0, 0 }

/* adds string str to storage tbl */
uint16_t storage_add_str(Storage *tbl, const char *str);

/* prints tbl as string storage */
void storage_print_str(const Storage *tbl);

/* adds instruction to tbl */
void storage_add_instr(Storage *tbl, uint8_t opcode, uint8_t oper);

/* returns current storage position address */
char *storage_current(const Storage *tbl);

/* skips 32 bits and returns pointer to it. Writes current offset to addr */
uint32_t *storage_skip32(Storage *tbl, uint32_t *addr);

/* adds byte to tbl */
void storage_add_byte(Storage *tbl, char byte);

/* prints tbl array with addr offset */
void storage_print_array(const Storage *tbl, uint32_t addr);

/* dumps storage to file */
void storage_write(const Storage *tbl, FILE *file);

#endif

