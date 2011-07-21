#include "addr.h"

void addr_init(Addr *addr, char *start, const RMDHeader *h)
{
	addr->start = (uint32_t *)(start + h->addr);
	addr->size  = RMD_SEG_ADDR_SIZE(*h) / 4;
}

