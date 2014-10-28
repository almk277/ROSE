/* Talking with user */

#ifndef PRINT_H
#define PRINT_H

#include "compiler.h"
struct Symbol;

/* level of debugging */
enum DebugLevel {
	DL_NONE,      /* silent */
	DL_DUMP,      /* tell about sections */
	DL_PERLINE,   /* tell about each file line too */
	DL_NUDE,      /* tell about internal structures too */
};

/* prints out error in printf format and finishes program */
C_ATTR_PRINTF(1, 2) C_ATTR_NORETURN
void error(const char *fmt, ...);

void error_symbol(const struct Symbol *sym, const char *msg);

/* prints information about current source line
 * if requested by verbosity level */
C_ATTR_PRINTF(1, 2)
void debug_line(const char *fmt, ...);

/* prints information about file 'name' error */
C_ATTR_NORETURN
void file_error(const char *name);

/* prints information about writing to output file error */
C_ATTR_NORETURN
void file_write_error(void);

#endif

