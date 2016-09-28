/*
 * Heap is continuous memory area used for object allocation.
 */

#ifndef ROSE_HEAP_H
#define ROSE_HEAP_H

#include "rmd.h"
#include "config.h"

#define RD_null 0

/*
 * On 32-bit platform, ROSE reference is native pointer.
 * On platform with bigger pointer (64-bit),
 * reference is an offset from heap start.
 * For generalization, use offsets in any case, but on 32-bit
 * heap starts at NULL, and compiler drop this out.
 */
#define HEAP_BASE (ROSE_LARGE ? hstart : (char*)0)

#define ptr_to_ref(ptr)  ((R_Word)((char*)(ptr) - HEAP_BASE))

#define ref_to_ptr(ref)  ((void*)(HEAP_BASE + ref))

/* Parent for any heap object */
typedef struct RefData {
	R_Word size; /* object size (including this structure itself */
} RefData;

extern char *hstart; /* current heap start */

/* creates heap */
void heap_create(void);

/* frees heap */
void heap_destroy(void);

/* allocates and returns new heap object of given size */
RefData *heap_alloc(R_Word size);

/* runs garbage collector */
void heap_run_gc(void);

#endif

