#include "hash.h"
#include "print.h"
#include "mm.h"
#include <string.h>
#include <stdio.h>

extern int verbose;
static MM_DECL(HashEntry, 32);

static HashEntry *hashentry_new(const String *s)
{
	HashEntry *e = mm_alloc(HashEntry);
	e->string = string_new(s->data, s->len);
	return e;
}

static void hashentry_delete(HashEntry *e)
{
	string_delete(e->string);
	mm_free(HashEntry, e);
}

static uint8_t hash(const String *s)
{
	uint8_t h = 0;
	int i = s->len;
	while(i--)
		h += s->data[i];
	return h;
}

static HashEntry *list_find(const ListHead *head, const String *s)
{
	HashEntry *ent;
	SLIST_FOREACH(ent, head, le)
		if(ent->string->len == s->len
				&& !strncmp(ent->string->data, s->data, s->len))
			return ent;
	return 0;
}

HashEntry *hash_find(const Hash *h, const String *s)
{
	uint8_t idx = hash(s);
	const ListHead *list = &h->heads[idx];
	if(!SLIST_EMPTY(list))
		return list_find(list, s);
	return 0;
}

int hash_get(const Hash *h, const String *s)
{
	const HashEntry *e = hash_find(h, s);
	if(e)
		return e->data.u32;
	return -1;
}

uint32_t hash_get_or_die(const Hash *h, const String *s)
{
	const HashEntry *e = hash_find(h, s);
	if(!e)
		error("%s: variable not found", s->data);
	return e->data.u32;
}

HashEntry *hash_add(Hash *h, const String *s)
{
	HashEntry *ent;
	uint8_t idx;

	if(hash_find(h, s))
		error("%s: name defined already", s->data);
	ent = hashentry_new(s);
	idx = hash(s);
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

