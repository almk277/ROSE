#include "str.h"

#include "mm.h"
#include "common.h"
#include <stdlib.h>
#include <string.h>

#define STR_MAX_LEN 255
typedef struct string {
	unsigned int len;
	char str[1];
} String;

#define SIZE_TYPE(size)   struct { char string[size]; }

typedef SIZE_TYPE(8) str8;
typedef SIZE_TYPE(16) str16;
typedef SIZE_TYPE(32) str32;
typedef SIZE_TYPE(64) str64;

static MM_DECL(str8, 32);
static MM_DECL(str16, 16);
static MM_DECL(str32, 8);
static MM_DECL(str64, 4);

static char *strpool_alloc(size_t size)
{
	if(size <= 8)
		return (char*)mm_alloc(str8);
	else if(size <= 16)
		return (char*)mm_alloc(str16);
	else if(size <= 32)
		return (char*)mm_alloc(str32);
	else if(size <= 64)
		return (char*)mm_alloc(str64);
	return malloc(size);
}

char *str_clone(const char *str)
{
	size_t len = strlen(str);
	char *s;
	if(len > STR_MAX_LEN)
		error("too long name");
	s = strpool_alloc(len + 1);
	if(!s)
		error("out of memory");
	return strcpy(s, str);
}

void str_free(char *str)
{
	size_t size = strlen(str);
	if(size <= 8)
		mm_free(str8, str);
	else if(size <= 16)
		mm_free(str16, str);
	else if(size <= 32)
		mm_free(str32, str);
	else if(size <= 64)
		mm_free(str64, str);
	else
		free(str);
}

