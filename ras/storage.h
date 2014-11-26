/* storage is a byte queue, implemented as dynamic list of byte arrays */

#ifndef STORAGE_H
#define STORAGE_H

#include "rmd.h"
#include "queue.h"
#include <stdint.h>
struct Symbol;

typedef struct Slice Slice;

SIMPLEQ_HEAD(QueueHead, Slice);

/* type descriptor, must be initialized */
typedef struct Storage {
	struct QueueHead head;
	Slice *current;
	RA_Array len;
} Storage;

/* initializer for Storage */
#define STORAGE_INITIALIZER(table) \
	{ SIMPLEQ_HEAD_INITIALIZER(table.head), 0, 0 }

/* Guarantees that tbl has at least size bytes free */
void storage_enlarge(Storage *tbl, RA_Array size);

void storage_put1byte(Storage *s, int8_t  word);
void storage_put2byte(Storage *s, int16_t word);
void storage_put4byte(Storage *s, int32_t word);
void storage_put_error(const char *s, const char *v, int len);

/* puts the word to the storage */
#define storage_put(storage, word) do { \
	if(sizeof(word) == 1) storage_put1byte((storage), (word)); \
	else if(sizeof(word) == 2) storage_put2byte((storage), (word)); \
	else if(sizeof(word) == 4) storage_put4byte((storage), (word)); \
	else storage_put_error(#storage, #word, sizeof(word)); \
} while(0)

/* adds symbol to storage tbl */
RA_Array storage_add_symbol(Storage *tbl, const struct Symbol *symbol);

/* returns current storage position address */
char *storage_current(const Storage *tbl);

/* dumps storage to file */
void storage_write(const Storage *tbl);

/* Begins new array in tbl. Returns it's start offset */
RA_Array array_begin(Storage *tbl);

/* adds byte to current array */
void array_add_byte(char byte);

/* ends current array */
void array_end(void);

#endif

