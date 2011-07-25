#include "addrcat.h"

void addrcat_add_tbl(AddrCat *cat, AddrTbl *tbl)
{
	cat->cat[cat->top++] = tbl;
}

