#include "heap.h"
#include <stdlib.h>

char *hstart, *hcur;

void heap_create(void)
{
	hstart = hcur = malloc(heap_size());
}

void heap_destroy(void)
{
	free(hstart);
}

RefData *heap_alloc(R_Word size)
{
	RefData *d = (RefData*)hcur;
	d->size = size;
	hcur += size;
	return d;
}

void heap_run_gc(void)
{
}

