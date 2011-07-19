#include "ptbl.h"
#include <string.h>

void ptbl_init(Ptbl *ptbl, char *start, const RMDHeader *h)
{
	ptbl->start = (RMDProcedure *)(start + h->ptbl);
	ptbl->size  = RMD_SEG_PTBL_SIZE(*h) / sizeof(RMDProcedure);
}

