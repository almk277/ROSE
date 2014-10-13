/* storage is list of byte arrays.
 * It grows dynamically when the size is not enough */

#ifndef STORAGE_H
#define STORAGE_H

#include "queue.h"
#include "symbol.h"
#include <stdint.h>

/* one array length */
#define SLICE_LEN  (2 * 1024)

typedef struct Slice Slice;

SIMPLEQ_HEAD(QueueHead, Slice);

/* type descriptor, must be initialized */
typedef struct Storage {
	struct QueueHead head;
	Slice *current;
	uint32_t len;
} Storage;

/* initializer for Storage */
#define STORAGE_INITIALIZER(table) \
	{ SIMPLEQ_HEAD_INITIALIZER(table.head), 0, 0 }

/* Guarantees that tbl has at least size bytes free */
void storage_enlarge(Storage *tbl, uint32_t size);

void storage_put1byte(Storage *s, char byte);

void storage_put2byte(Storage *s, int16_t word);

void storage_put4byte(Storage *s, int32_t word);

/* adds symbol to storage tbl */
uint32_t storage_add_symbol(Storage *tbl, const Symbol *symbol);

/* prints tbl as string storage */
void storage_print_str(const Storage *tbl);

/* returns current storage position address */
char *storage_current(const Storage *tbl);

/* dumps storage to file */
void storage_write(const Storage *tbl);

uint32_t array_begin(Storage *tbl);

void array_add_byte(char byte);

#endif

