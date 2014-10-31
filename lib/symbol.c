#include "symbol.h"
#include "mm.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct Symbol {
	SymbolSize len;
	char data[1];
};

#define DEFPOOL(size, count) \
	typedef Symbol Symbol ## size; \
	static mmPoolStruct MM_POOL(Symbol ## size) = { 0, count, size + 1 };

DEFPOOL(4,  64);
DEFPOOL(8,  32);
DEFPOOL(16, 16);
DEFPOOL(32, 8);

Symbol *symbol_new(const char *str, int len)
{
	Symbol *s;
	if(len < 4)
		s = mm_alloc(Symbol4);
	else if(len < 8)
		s = mm_alloc(Symbol8);
	else if(len < 16)
		s = mm_alloc(Symbol16);
	else if(len < 32)
		s = mm_alloc(Symbol32);
	else
		s = malloc(len + 1);
	memcpy(s->data, str, len);
	s->len = len;
	return s;
}

void symbol_delete(Symbol *s)
{
	int len = s->len;
	if(len < 4)
		mm_free(Symbol4, s);
	else if(len < 8)
		mm_free(Symbol8, s);
	else if(len < 16)
		mm_free(Symbol16, s);
	else if(len < 32)
		mm_free(Symbol32, s);
	else
		free(s);
}

SymbolSize symbol_length(const Symbol *s)
{
	return s->len;
}

SymbolSize symbol_store_length(const Symbol *s)
{
	return s->len + 1;
}

int symbol_compare(const Symbol *s1, const Symbol *s2)
{
	return (s1->len == s2->len) && (memcmp(s1->data, s2->data, s1->len) == 0);
}

unsigned char symbol_hash(const Symbol *s)
{
	unsigned char h = 0;
	int i = s->len;
	while(i--)
		h += s->data[i];
	return h;
}

void symbol_print(const Symbol *s)
{
	fwrite(s->data, s->len, 1, stdout);
}

void symbol_print_to_file(const Symbol *s, FILE *file)
{
	fwrite(s->data, s->len, 1, file);
}

void symbol_copy_to(const Symbol *s, char *buffer)
{
	*buffer++ = s->len;
	memcpy(buffer, s->data, s->len);
}

int symbol_split_colon(const Symbol *s, Symbol **s1, Symbol **s2)
{
	char *colon = strchr(s->data, ':');
	if(!colon)
		return 0;
	/* split up fullname into module and procedure names */
	*s1 = symbol_new(s->data, colon - s->data);
	*s2 = symbol_new(colon + 1, s->data + s->len - colon);
	return 1;
}

static const char s_empty[] = { 0 };
static const char s_main[] = { 4, 'm', 'a', 'i', 'n' };

const struct SymbolStore symbol_store = {
	(const Symbol*)&s_empty,
	(const Symbol*)&s_main
};

