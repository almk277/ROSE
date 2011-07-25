#include "addrtbl.h"
#include <stdlib.h>

AddrTbl addrtbl_first;

AddrTbl *addrtbl_new(void)
{
	return calloc(1, sizeof(AddrTbl));
}

void addrtbl_delete(AddrTbl *tbl)
{
	free(tbl);
}

uint32_t addrtbl_add_ptr(AddrTbl *tbl, Pointer ptr)
{
	tbl->tbl[tbl->top++] = ptr;
	return tbl->top;
}

