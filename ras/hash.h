/* simple hash with string keys */
/* 8-bit string hash is index into array of list heads */

#ifndef HASH_H
#define HASH_H

#include <stdint.h>
#include "cstring.h"
#include "queue.h"

/* queue array length */
#define HASH_LEN 256

/* head for collision list */
typedef SLIST_HEAD(, HashEntry) ListHead;

/* hash descriptor */
typedef struct Hash {
	ListHead heads[HASH_LEN];
	int count;
} Hash;

/* object stored in hash */
typedef struct HashEntry {
	String *string;
	union {
		uint8_t u8;
		//uint16_t u16;
		uint32_t u32;
	} data;
	SLIST_ENTRY(HashEntry) le;
} HashEntry;

/* adds string to hash h, checks it for uniqueness
 * and returns reference to it's entry */
HashEntry *hash_add(Hash *h, const String *string);

/* looks for name in hash h */
HashEntry *hash_find(const Hash *h, const String *string);

/* returns index for name in hash h, or -1 if not found */
int hash_get(const Hash *h, const String *string);

/* returns index for name s in hash h, or die if not found */
uint32_t hash_get_or_die(const Hash *h, const String *s);

/* clears hash h */
void hash_clear(Hash *h);

/* prints hash content, calling print for every element */
void hash_print(const Hash *h, void (*print)(const HashEntry *ent));

#endif

