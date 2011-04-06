/* simple hash with string keys */
/* 8-bit string hash is index into array of list heads */

#ifndef HASH_H
#define HASH_H

#include <stdint.h>
#include "queue.h"

/* queue array length */
#define HASH_LEN 256

/* head for collision list */
typedef SLIST_HEAD(, HashEntry) ListHead;

/* hash descriptor */
typedef struct Hash {
	ListHead heads[HASH_LEN];
	uint8_t count;
} Hash;

/* object stored in hash */
typedef struct HashEntry {
	char *name;
	uint32_t data;
	SLIST_ENTRY(HashEntry) le;
} HashEntry;

/* adds name to hash h and assigns to it new index */
HashEntry *hash_add(Hash *h, const char *name);

/* looks for name in hash h */
HashEntry *hash_find(const Hash *h, const char *name);

/* returns index for name in hash h, or -1 if not found */
int hash_get(const Hash *h, const char *name);

/* clears hash h */
void hash_clear(Hash *h);

/* prints hash content, calling print for every element */
void hash_print(const Hash *h, void (*print)(const HashEntry *ent));

#endif

