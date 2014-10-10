#include "print.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

extern int yylineno;
extern int verbose;
extern const char *output_name;

void error(const char *fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	fprintf(stderr, "error: line %d: ", yylineno);
	vfprintf(stderr, fmt, va);
	va_end(va);
	putchar('\n');
	exit(1);
}

#ifdef DEBUG
void debug_line(const char *fmt, ...)
{
	if(verbose >= DL_PERLINE) {
		va_list va;
		va_start(va, fmt);
		fprintf(stderr, "line %d: ", yylineno);
		vfprintf(stderr, fmt, va);
		va_end(va);
		putchar('\n');
	}
}
#endif

void file_error(const char *name)
{
	perror(name);
	exit(1);
}

void file_write_error()
{
	file_error(output_name);
}

