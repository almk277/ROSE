/* Simple hash table with symbol keys. */

#ifndef ROSE_SYMTBL_H
#define ROSE_SYMTBL_H

#include "symbol.h"
#include <stdint.h>
#include "compiler.h"

/* Symbol table itself */
typedef struct SymbolTable SymbolTable;

/* Value, assosiated with symbol in SymbolTable */
typedef union SymbolValue {
	int i;
	int32_t i32;
	uint32_t u32;
	void *p;
} SymbolValue;

/* static initializer for SymbolTable */
#define SYMBOLTABLE_INITIALIZER { {}, 0 }

/* initializes symbol table */
void symtbl_init(SymbolTable *tbl);

/* returns number of elements in tbl */
C_ATTR_CONST
int symtbl_size(const SymbolTable *tbl);

/* adds symbol to tbl, if not there yet, and returns pointer to it's value.
 * Otherwise, returns NULL */
SymbolValue *symtbl_add_unique(SymbolTable *tbl, Symbol *symbol);

/* adds symbol to tbl, if not there yet. Returns pointer to it's value. */
SymbolValue *symtbl_add_or_get(SymbolTable *tbl, Symbol *symbol);

/* searches for symbol in tbl and returns pointer to it's value.
 * Otherwise, returns NULL */
C_ATTR_PURE
SymbolValue *symtbl_find(const SymbolTable *tbl, Symbol *symbol);

/* clears tbl */
void symtbl_clear(SymbolTable *tbl);

/* calls fn for every element in tbl */
void symtbl_foreach(const SymbolTable *tbl,
		void (*fn)(Symbol *sym, const SymbolValue *val));



/***  Not for direct use!  ***/

#include "queue.h"

typedef SLIST_ENTRY(SymbolEntry) SymbolSlistEntry;
typedef SLIST_HEAD(, SymbolEntry) SymbolSlistHead;
struct SymbolEntry {
	Symbol *symbol;
	SymbolValue value;
	SymbolSlistEntry le;
};

struct SymbolTable {
	SymbolSlistHead heads[256];
	int size;
};

#endif

