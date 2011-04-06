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

#include "mm.h"
#include <stdlib.h>

/* Common variable. Defaults to zero, but can be redefined by user */
void *(*mm_on_error)(struct mm_pool_struct *);

#ifdef TEST
static int alloc_count = 0;
#endif

/* allocates new memory in pool via malloc.
 * pool - pointer to pool structure;
 * size - memory chunk size;
 * returns pointer to new chunk. */
void *_mm_alloc(struct mm_pool_struct *pool)
{
	size_t const size = pool->size;
	unsigned poolsize = pool->count;
	char *mem = malloc(size * poolsize);
#ifdef TEST
	++alloc_count;
#endif
	if(unlikely(mem == NULL)) {
		poolsize = MM_MIN_SIZE;
		mem = malloc(size * MM_MIN_SIZE);
		if(unlikely(mem == NULL))
			return mm_on_error ? mm_on_error(pool) : NULL;
	}

	pool->head = mem;
	for(poolsize -= 2; poolsize != 0; --poolsize, mem += size)
		*(void **)mem = mem + size;
	*(void **)mem = NULL;
	return mem + size;
}

/* Some tests */
#ifdef TEST

#include <assert.h>

#define N 10

MM_DECL(int, N);
MM_EXTERN(int);

static int *addr[2 * N + 1];

static void init(int k)
{
	addr[k] = mm_alloc(int);
	*(addr[k]) = k;
}

int main(void)
{
	int i;

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
		mm_free(int, addr[i]);

	/* Now check if we get the same addresses in opposite order */
	for(i = 2 * N; i >= 0; --i)
		assert(mm_alloc(int) == addr[i]);

	return 0;
}

/* Functions to profile */

int *int_get(void)
{
	return mm_alloc(int);
}

void int_put(int *p)
{
	mm_free(int, p);
}
#endif

