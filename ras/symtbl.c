#include "hash.h"
#include "print.h"
#include "mm.h"
#include <string.h>
#include <stdio.h>

extern int verbose;
static MM_DECL(HashEntry, 32);

static HashEntry *hashentry_new(const Symbol *s)
{
	HashEntry *e = mm_alloc(HashEntry);
	e->symbol = s;
	return e;
}

static void hashentry_delete(HashEntry *e)
{
	symbol_delete((Symbol*)e->symbol);
	mm_free(HashEntry, e);
}

static HashEntry *list_find(const ListHead *head, const Symbol *s)
{
	HashEntry *ent;
	SLIST_FOREACH(ent, head, le)
		if(symbol_compare(ent->symbol, s))
			return ent;
	return 0;
}

HashEntry *hash_find(const Hash *h, const Symbol *s)
{
	uint8_t idx = symbol_hash(s);
	const ListHead *list = &h->heads[idx];
	if(!SLIST_EMPTY(list))
		return list_find(list, s);
	return 0;
}

int hash_get(const Hash *h, const Symbol *s)
{
	const HashEntry *e = hash_find(h, s);
	if(e)
		return e->data.u32;
	return -1;
}

uint32_t hash_get_or_die(const Hash *h, const Symbol *s)
{
	const HashEntry *e = hash_find(h, s);
	if(!e)
		error_symbol(s, "variable not found");
	return e->data.u32;
}

HashEntry *hash_add(Hash *h, const Symbol *s)
{
	HashEntry *ent;
	uint8_t idx;

	if(hash_find(h, s))
		error_symbol(s, "name defined already");
	ent = hashentry_new(s);
	idx = symbol_hash(s);
	SLIST_INSERT_HEAD(&h->heads[idx], ent, le);
	++h->count;
	ent->data.u32 = h->count;
	return ent;
}

void hash_clear(Hash *h)
{
	int i;
	for(i = 0; i != HASH_LEN; ++i) {
		ListHead *head = &h->heads[i];
		while(!SLIST_EMPTY(head)) {
			hashentry_delete(SLIST_FIRST(head));
			SLIST_REMOVE_HEAD(head, le);
		}
	}
	h->count = 0;
}

#ifdef DEBUG
void hash_print(const Hash *h, void (*print)(const HashEntry *ent))
{
	int i;
	for(i = 0; i != HASH_LEN; ++i) {
		const ListHead *head = &h->heads[i];
		if(!SLIST_EMPTY(head)) {
			const HashEntry *ent;
			if(verbose >= DL_NUDE)
				printf("\n - ");
			SLIST_FOREACH(ent, head, le)
				print(ent);
		}
	}
	putchar('\n');
}
#endif

