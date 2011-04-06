#include "common.h"
#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>

int lineno = 0;
int verbose = 0;
uint32_t offset = 0;
const struct HashEntry *cur_proc;

void error(const char *fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	fprintf(stderr, "error: line %d: ", lineno);
	vfprintf(stderr, fmt, va);
	va_end(va);
	putchar('\n');
	exit(1);
}

void unexpect_sym(const char *fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	fprintf(stderr, "error: line %d: unexpected symbols ", lineno);
	vfprintf(stderr, fmt, va);
	va_end(va);
	putchar('\n');
	exit(1);
}

void debug_line(const char *fmt, ...)
{
	va_list va;
	if(DL_PERLINE > verbose)
		return;
	va_start(va, fmt);
	fprintf(stderr, "line %d: ", lineno);
	vfprintf(stderr, fmt, va);
	va_end(va);
	putchar('\n');
}

void warning(const char *message)
{
	fprintf(stderr, "warning: line %d: %s\n", lineno, message);
}

void file_write_error(void)
{
	fprintf(stderr, "file write error\n");
}

void check_name(const char *name)
{
	if(!isalpha(*name))
		goto error;
	for(++name; *name; ++name)
		if(!(isalnum(*name) || *name == '_'))
			goto error;
	return;
error:
	error("invalid name '%s'", name);
}

void pass_spaces(char **pos)
{
	while(**pos && isspace(**pos))
		++*pos;
}

void go_to_space(char **pos)
{
	while(**pos && !isspace(**pos))
		++*pos;
}

void go_to_word_end(char **pos)
{
	while(**pos && (isalnum(**pos) || **pos == '_'))
		++*pos;
}

