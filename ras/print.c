#include "print.h"
#include "symbol.h"
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
	fputc('\n', stderr);
	exit(1);
}

void error_symbol(const Symbol *sym, const char *msg)
{
	fprintf(stderr, "error: line %d: ", yylineno);
	symbol_print_to_file(sym, stderr);
	fprintf(stderr, ": %s\n", msg);
	exit(1);
}

void debug_line(const char *fmt, ...)
{
	if(verbose >= DL_PERLINE) {
		va_list va;
		va_start(va, fmt);
		printf("line %d: ", yylineno);
		vprintf(fmt, va);
		va_end(va);
		putchar('\n');
	}
}

void file_error(const char *name)
{
	perror(name);
	exit(1);
}

void file_write_error()
{
	file_error(output_name);
}

