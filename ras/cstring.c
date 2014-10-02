#include "cstring.h"
#include "mm.h"
#include <stdlib.h>
#include <string.h>

#define DEFPOOL(size, count) \
	typedef String String ## size; \
	static mmPoolStruct MM_POOL(String ## size) = { 0, count, sizeof(int) + size };

DEFPOOL(4,  64);
DEFPOOL(8,  32);
DEFPOOL(16, 16);
DEFPOOL(32, 8);

String *string_new(const char *str, int len)
{
	String *s;
	if(len <= 4)
		s = mm_alloc(String4);
	else if(len <= 8)
		s = mm_alloc(String8);
	else if(len <= 16)
		s = mm_alloc(String16);
	else if(len <= 32)
		s = mm_alloc(String32);
	else
		s = malloc(sizeof(int) + len);
	memcpy(s->data, str, len);
	s->len = len;
	return s;
}

void string_delete(String *s)
{
	int len = s->len;
	if(len <= 4)
		mm_free(String4, s);
	else if(len <= 8)
		mm_free(String8, s);
	else if(len <= 16)
		mm_free(String16, s);
	else if(len <= 32)
		mm_free(String32, s);
	else
		free(s);
}

