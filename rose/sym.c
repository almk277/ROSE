#include "sym.h"

void sym_init(Sym *sym, const char *start, const RMDHeader *h)
{
	sym->start = start + h->sym;
	sym->size  = RMD_SEG_SYM_SIZE(*h);
}

