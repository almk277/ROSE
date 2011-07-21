#include "cnst.h"

void cnst_init(Cnst *cnst, char *start, const RMDHeader *h)
{
	cnst->start = (int32_t *)(start + h->cnst);
	cnst->size  = RMD_SEG_CONSTS_SIZE(*h) / 4;
}

