#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include "compiler.h"
#include "hash.h"

#define ROSE_VERSION_MAJ   1
#define ROSE_VERSION_MIN   0

#define MAX_LINE_LEN      128

enum SectionType {
	SECT_NONE,
	SECT_PROC,
	SECT_CONST,
	SECT_DATA,
};

enum DebugLevel {
	DL_NONE,
	DL_DUMP,
	DL_PERLINE,
	DL_NUDE,
};

extern enum SectionType cur_sect;
extern int verbose;
extern int lineno;
extern uint32_t offset;
extern const struct HashEntry *cur_proc;

C_ATTR_PRINTF(1, 2)
void error(const char *fmt, ...);

C_ATTR_PRINTF(1, 2)
void unexpect_sym(const char *fmt, ...);

void warning(const char *message);

C_ATTR_PRINTF(1, 2)
void debug_line(const char *fmt, ...);

void file_write_error(void);

void pass_spaces(char **pos);

void go_to_space(char **pos);

void go_to_word_end(char **pos);

void check_name(const char *name);

#endif

