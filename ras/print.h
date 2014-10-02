/* Talking with user */

#ifndef PRINT_H
#define PRINT_H

#include "compiler.h"

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

/* prints information about current source line
 * if requested by verbosity level */
#ifdef DEBUG
C_ATTR_PRINTF(1, 2)
void debug_line(const char *fmt, ...);
#else
#define debug_line(fmt, ...)
#endif

/* prints information about file 'name' error */
C_ATTR_NORETURN
void file_error(const char *name);

/* prints information about writing to output file error */
C_ATTR_NORETURN
void file_write_error(void);

#endif

