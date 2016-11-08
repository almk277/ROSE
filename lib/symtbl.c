#include "symtbl.h"
#include "mm.h"
#include <stddef.h>
#include <string.h>

typedef struct SymbolEntry SymbolEntry;

static MM_DECL(SymbolEntry, 32);
#define FOREACH(idx, tbl) \
	for(idx = 0; idx != sizeof(tbl->heads) / sizeof(tbl->heads[0]); ++idx)

static unsigned char symbol_hash(Symbol *s)
{
	unsigned char h = 0;
	const char *c;
	for (c = s; *c; ++c)
		h += *c;
	return h;
}

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

static SymbolEntry *list_find(const SymbolSlistHead *head, Symbol *s)
{
	SymbolEntry *ent;
	SLIST_FOREACH(ent, head, le)
		if(strcmp(ent->symbol, s) == 0)
			return ent;
	return NULL;
}

SymbolValue *symtbl_find(const SymbolTable *tbl, Symbol *s)
{
	unsigned char idx = symbol_hash(s);
	const SymbolSlistHead *list = &tbl->heads[idx];
	SymbolEntry *e = list_find(list, s);
	if(e)
		return &e->value;
	return NULL;
}

static SymbolValue *add(SymbolTable *tbl, SymbolSlistHead *list, Symbol *s)
{
	SymbolEntry *e = mm_alloc(SymbolEntry);
	e->symbol = s;
	SLIST_INSERT_HEAD(list, e, le);
	++tbl->size;
	return &e->value;
}

SymbolValue *symtbl_add_unique(SymbolTable *tbl, Symbol *symbol)
{
	unsigned char idx = symbol_hash(symbol);
	SymbolSlistHead *list = &tbl->heads[idx];
	SymbolEntry *ent = list_find(list, symbol);
	if(ent)
		return NULL;
	return add(tbl, list, symbol);
}

SymbolValue *symtbl_add_or_get(SymbolTable *tbl, Symbol *symbol)
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
		void (*fn)(Symbol *sym, const SymbolValue *val))
{
	int i;
	FOREACH(i, tbl) {
		const SymbolSlistHead *head = &tbl->heads[i];
		const SymbolEntry *ent;
		SLIST_FOREACH(ent, head, le)
			fn(ent->symbol, &ent->value);
	}
}

