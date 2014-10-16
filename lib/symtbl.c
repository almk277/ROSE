#include "symtbl.h"
#include "symbol.h"
#include "mm.h"
#include <stddef.h>

typedef struct SymbolEntry SymbolEntry;

static MM_DECL(SymbolEntry, 32);
#define FOREACH(idx, tbl) \
	for(idx = 0; idx != sizeof(tbl->heads) / sizeof(tbl->heads[0]); ++idx)

void symtbl_init(SymbolTable *tbl)
{
	int i;
	FOREACH(i, tbl)
		SLIST_INIT(&tbl->heads[i]);
	tbl->size = 0;
}

int symtbl_size(const SymbolTable *tbl)
{
	return tbl->size;
};

static SymbolEntry *list_find(const SymbolSlistHead *head, const Symbol *s)
{
	SymbolEntry *ent;
	SLIST_FOREACH(ent, head, le)
		if(symbol_compare(ent->symbol, s))
			return ent;
	return NULL;
}

SymbolValue *symtbl_find(const SymbolTable *tbl, const Symbol *s)
{
	unsigned char idx = symbol_hash(s);
	const SymbolSlistHead *list = &tbl->heads[idx];
	SymbolEntry *e = list_find(list, s);
	if(e)
		return &e->value;
	return NULL;
}

static SymbolValue *add(SymbolTable *tbl, SymbolSlistHead *list, const Symbol *s)
{
	SymbolEntry *e = mm_alloc(SymbolEntry);
	e->symbol = s;
	SLIST_INSERT_HEAD(list, e, le);
	++tbl->size;
	return &e->value;
}

SymbolValue *symtbl_add_unique(SymbolTable *tbl, const Symbol *symbol)
{
	unsigned char idx = symbol_hash(symbol);
	SymbolSlistHead *list = &tbl->heads[idx];
	SymbolEntry *ent = list_find(list, symbol);
	if(ent)
		return NULL;
	return add(tbl, list, symbol);
}

SymbolValue *symtbl_add_or_get(SymbolTable *tbl, const Symbol *symbol)
{
	unsigned char idx = symbol_hash(symbol);
	SymbolSlistHead *list = &tbl->heads[idx];
	SymbolEntry *ent = list_find(list, symbol);
	if(ent)
		return &ent->value;
	return add(tbl, list, symbol);
}

void symtbl_clear(SymbolTable *tbl)
{
	int i;
	FOREACH(i, tbl) {
		SymbolSlistHead *head = &tbl->heads[i];
		while(!SLIST_EMPTY(head)) {
			//TODO delete symbol?
			mm_free(SymbolEntry, SLIST_FIRST(head));
			SLIST_REMOVE_HEAD(head, le);
		}
	}
	tbl->size = 0;
}

void symtbl_foreach(const SymbolTable *tbl,
		void (*fn)(const struct Symbol *sym, const SymbolValue *val))
{
	int i;
	FOREACH(i, tbl) {
		const SymbolSlistHead *head = &tbl->heads[i];
		const SymbolEntry *ent;
		SLIST_FOREACH(ent, head, le)
			fn(ent->symbol, &ent->value);
	}
}

