#include "text.h"

void text_init(Text *text, char *start, const RMDHeader *h)
{
	text->start = start + h->text;
	text->size  = RMD_SEG_TEXT_SIZE(*h);
}

