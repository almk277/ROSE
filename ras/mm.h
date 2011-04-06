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

/*  Trivial memory pool  */

#ifndef MM_H
#define MM_H

#include "compiler.h"
#include "macros.h"
/* for size_t */
#include <stddef.h>

/******************  public  ****************/

#define    MM_MIN_SIZE		3

/* Initialises memory pool for given type with count memory chunks.
 * Type must be specified in one word.
 * Prerequisites:
 *   sizeof(type) >= sizeof(void *)
 *   size >= MM_MIN_SIZE */
#define MM_DECL(type, count) \
	struct mm_pool_struct _MM_POOL(type) = { 0, count, sizeof(type) }; \
	_MM_ASSERT(type, count)

/* Imports memory pool, declared with MM_DECL elsewhere */
#define MM_EXTERN(type) \
	extern struct mm_pool_struct _MM_POOL(type)

/* Returns free memory chunk from pool,
 * or NULL if there is no free memory
 * (or mm_on_error return value, see further) */
#define mm_alloc(type) \
	((type *)(likely(_MM_POOL(type).head) ?  \
		_mm_get_chunk(&_MM_POOL(type)): _mm_alloc(&_MM_POOL(type))))

/* Frees memory at addr and returns it to the pool for type. */
#define mm_free(type, addr) \
	C_STMT_BEGIN { \
		*(void **)addr = _MM_POOL(type).head; \
		_MM_POOL(type).head = addr; \
	} C_STMT_END

/* Pool descriptor. Might be used in mm_on_error */
struct mm_pool_struct {
	void *head;           /* free chunks list head */
	unsigned count;       /* maximum element count */
	size_t const size;    /* element size */
};

/* If defined as non-null address, function with this address
 * will be called when there is no free memory, and this return value
 * will be returned by mm_alloc */
extern void *(*mm_on_error)(struct mm_pool_struct *);

/********************  Internal stuff  *******************/

#define _MM_POOL(type)         mm_pool_ ## type

#define _MM_ASSERT(type, size) \
	C_STATIC_ASSERT(type ## _type_too_small, sizeof(type) >= sizeof(void *)); \
	C_STATIC_ASSERT(type ## _size_too_small, size >= MM_MIN_SIZE);

C_ATTR_MALLOC
void *_mm_alloc(struct mm_pool_struct *pool);

C_ATTR_MALLOC
static inline void *_mm_get_chunk(struct mm_pool_struct *pool)
{
	void *head = pool->head;
	pool->head = *(void **)head;
	return head;
}

#endif

