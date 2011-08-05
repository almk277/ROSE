#include "str.h"

void str_init(Str *str, const char *start, const RMDHeader *h)
{
	str->start = start + h->str;
	str->size  = RMD_SEG_STR_SIZE(*h);
}

