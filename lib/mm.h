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

/******************  public  ****************/

/* Usage: see an example in mm.c */

/* Object alignment in bytes. Can be redefined */
#ifndef MM_ALIGN
#define MM_ALIGN               sizeof(int)
#endif

/* Name of the pool for the given type */
#define MM_POOL(type)          mm_pool_ ## type

#define MM_POOL_INIT(type, count) \
	{ 0, count, _MM_OBJ_SIZE(sizeof(type)) }

#define mm_init_pool(type, count) \
	do { \
		mmPoolStruct pool = MM_POOL(type); \
		pool.head = 0; \
		pool.size = count; \
		pool.el_size = _MM_OBJ_SIZE(sizeof(count)); \
	} while(0)

/* Initialises memory pool for given type with count memory chunks.
 * Type must be specified in one word. */
#define MM_DECL(type, count) \
	mmPoolStruct MM_POOL(type) = MM_POOL_INIT(type, count)

/* Returns free memory chunk from pool,
 * or NULL if there is no free memory
 * (or mm_on_error return value, see further) */
#define mm_alloc(type) ((type *)mm_get_chunk(&MM_POOL(type)))

/* Frees memory at addr and returns it to the pool for type. */
#define mm_free(type, addr) \
	do { \
		*(void **)addr = MM_POOL(type).head; \
		MM_POOL(type).head = addr; \
	} while(0)

/* Pool descriptor. Might be used in mm_on_error */
typedef struct mm_pool_struct {
	void *head;             /* free chunks list head */
	unsigned size;          /* maximum element count */
	unsigned int el_size;   /* element size */
} mmPoolStruct;

/* If defined as non-null address, function with this address
 * will be called when there is no free memory, and it's return value
 * will be returned by mm_alloc */
extern void *(*mm_on_error)(struct mm_pool_struct *);

/********************  Internal stuff  *******************/

/* Align size on MM_ALIGN boundary */
#define _MM_OBJ_SIZE(size) \
	(size < sizeof(void*) ? sizeof(void*) : \
	 (size + MM_ALIGN - 1) & ~(MM_ALIGN - 1))

C_ATTR_MALLOC
static inline void *mm_get_chunk(struct mm_pool_struct *pool)
{
	void *head;
	void mm_pool_alloc(struct mm_pool_struct *pool);


	if(unlikely(!pool->head))
		mm_pool_alloc(pool);
	head = pool->head;
	pool->head = *(void **)head;

	return head;
}

#endif

