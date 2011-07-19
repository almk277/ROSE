#include "exp.h"

void exp_init(Exp *exp, char *start, const RMDHeader *h)
{
	exp->start = (RMDExport *)(start + h->exp);
	exp->size  = RMD_SEG_EXP_SIZE(*h) / sizeof(RMDExport);
}

