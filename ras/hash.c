#include "hash.h"
#include "str.h"
#include "common.h"
#include "mm.h"
#include <string.h>
#include <stdio.h>

static MM_DECL(HashEntry, 32);

static HashEntry *hashentry_new(const char *name)
{
	HashEntry *e = mm_alloc(HashEntry);
	if(!e)
		error("out of memory");
	e->name = str_clone(name);
	return e;
}

static void hashentry_delete(HashEntry *e)
{
	str_free(e->name);
	mm_free(HashEntry, e);
}

static uint8_t hash(const char *str)
{
	uint8_t h = 0;
	for(; *str; ++str)
		h += *str;
	return h;
}

static HashEntry *list_find(const ListHead *head, const char *name)
{
	HashEntry *ent;
	SLIST_FOREACH(ent, head, le)
		if(!strcmp(ent->name, name))
			return ent;
	return 0;
}

HashEntry *hash_find(const Hash *h, const char *name)
{
	uint8_t idx = hash(name);
	const ListHead *list = &h->heads[idx];
	if(!SLIST_EMPTY(list))
		return list_find(list, name);
	return 0;
}

int hash_get(const Hash *h, const char *name)
{
	const HashEntry *e = hash_find(h, name);
	if(e)
		return e->data;
	return -1;
}

HashEntry *hash_add(Hash *h, const char *name)
{
	HashEntry *ent;
	uint8_t idx;
	if(hash_find(h, name))
		error("name defined already");
	if(h->count == HASH_LEN)
		error("too many strings");
	ent = hashentry_new(name);
	ent->data = h->count++;
	idx = hash(name);
	SLIST_INSERT_HEAD(&h->heads[idx], ent, le);
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

