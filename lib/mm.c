/*
 * Copyright (c) 2011 Makarov Alexey
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the author nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

/***  Usage example  ***/

#if 0
#include "mm.h"
void example(void)
{
	typedef struct Object {
		char *name;
		int data;
	} Object;

	MM_DECL(Object, 10);

	Object *list[10];
	int i;

	for(i = 0; i != 10; ++i)
		list[i] = mm_alloc(Object);

	do_something(list);

	for(i = 0; i != 10; ++i)
		mm_free(Object, list[i]);
}
#endif

#include "mm.h"
#include <stdlib.h>
#include <stdio.h>

#define    MM_MIN_SIZE		4

/* Common variable. Defaults to zero, but can be redefined by user */
void *(*mm_on_error)(struct mm_pool_struct *);

#ifdef TEST
static int alloc_count = 0;
#endif

/* allocates new memory in pool via malloc.
 * pool - pointer to pool structure;
 * size - memory chunk size;
 * returns pointer to new chunk. */
void mm_pool_alloc(struct mm_pool_struct *pool)
{
	const unsigned size = pool->el_size;
	unsigned poolsize = pool->size;
	char *mem = malloc(size * poolsize);
#ifdef TEST
	++alloc_count;
#endif
	if(unlikely(mem == NULL)) {
		poolsize = MM_MIN_SIZE;
		mem = malloc(size * MM_MIN_SIZE);
		if(unlikely(mem == NULL)) {
			if(mm_on_error)
				mem = mm_on_error(pool);
			else {
				fputs(__FILE__ ": ERROR: Out of memory", stderr);
				exit(1);
			}
		}
	}

	pool->head = mem;
	for(--poolsize; poolsize != 0; --poolsize, mem += size)
		*(void **)mem = mem + size;
	*(void **)mem = NULL;
}

/* Some tests */
#ifdef TEST

#include <assert.h>

#define N 10

MM_DECL(long, N);

static long *addr[2 * N + 1];

static void init(long k)
{
	addr[k] = mm_alloc(long);
	*(addr[k]) = k;
}

int main(void)
{
	long i;

	/* Track when pool is filled */
	assert(alloc_count == 0);

	init(0);
	assert(alloc_count == 1);

	for(i = 1; i < N - 1; ++i)
		init(i);
	assert(alloc_count == 1);

	init(N - 1);
	assert(alloc_count == 1);

	init(N);
	assert(alloc_count == 2);

	for(i = N + 1; i < 2 * N - 1; ++i)
		init(i);
	assert(alloc_count == 2);
	
	init(2 * N - 1);
	assert(alloc_count == 2);

	init(2 * N);
	assert(alloc_count == 3);

	for(i = 0; i <= 2 * N; ++i)
		mm_free(long, addr[i]);

	/* Now check if we get the same addresses in opposite order */
	for(i = 2 * N; i >= 0; --i)
		assert(mm_alloc(long) == addr[i]);

	return 0;
}

/* Functions to profile */

long *int_get(void)
{
	return mm_alloc(long);
}

void int_put(long *p)
{
	mm_free(long, p);
}
#endif

