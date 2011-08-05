/* Some common types, variables and functions */

#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include "compiler.h"
#include "hash.h"

/* ROSE version */
#define ROSE_VERSION_MAJ   1
#define ROSE_VERSION_MIN   0

/* section types */
enum SectionType {
	SECT_NONE,     /* no section */
	SECT_PROC,     /* #text      */
	SECT_CONST,    /* #const     */
	SECT_STR,      /* #str       */
	SECT_DATA,     /* #data      */
};

/* level of debugging */
enum DebugLevel {
	DL_NONE,
	DL_DUMP,
	DL_PERLINE,
	DL_NUDE,
};

extern enum SectionType cur_sect;         /* current section */
extern int verbose;                       /* verbosity level */
extern int lineno;                        /* current source line number */
extern uint32_t offset;                   /* current #text offset */
extern const struct HashEntry *cur_proc;  /* current procedure */
extern uint32_t *array_len;               /* current string length field */

/* prints out error in printf format and finishes program */
C_ATTR_PRINTF(1, 2)
void error(const char *fmt, ...);

/* prints out error about unexpected symbol in printf format
 * and finishes program */
C_ATTR_PRINTF(1, 2)
void unexpect_sym(const char *fmt, ...);

/* prints warning */
void warning(const char *message);

/* prints information about current source line
 * if requested by verbosity level */
C_ATTR_PRINTF(1, 2)
void debug_line(const char *fmt, ...);

/* prints out about file write error and finishes program */
void file_write_error(void);

/* changes pos to point to first non-space */
void pass_spaces(char **pos);

/* changes pos to point to first space */
void go_to_space(char **pos);

/* changes pos to point to current word end */
void go_to_word_end(char **pos);

/* returns if name is a valid name */
int name_is_valid(const char *name);

/* checks if name is valid; prints error message and finishes program
 * if it is not */
void check_name(const char *name);

void check_word_is_last(char *word, const char *msg);

#endif

