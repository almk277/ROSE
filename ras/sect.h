/* various RMD tables */

#ifndef SECT_H
#define SECT_H

#include "rmd.h"
struct Symbol;

/* adds data with name */
void data_add(const struct Symbol *name);

/* adds module with name to #mtbl */
void module_add(const struct Symbol *name);

/* sets version for last imported module */
void module_set_version(const char *version);

/* adds extern name to #imp */
void imp_add(const struct Symbol *name);

/* sets module name */
void header_set_name(const struct Symbol *name);

/* sets module version */
void header_set_version(const char *version);

/* sets module parent */
void header_set_parent(const struct Symbol *name);

/* fills module header with current values */
void header_fill(void);

/* begins new #str string */
void str_begin(const struct Symbol *name);

/* adds a byte c to current #str string */
void str_add_char(char c);

/* ends #str string */
void str_end(void);

/* adds procedure name to #ptbl */
void ptbl_add(const struct Symbol *name);

/* adds argument with name */
void arg_add(const struct Symbol *name);

/* adds variable with name */
void var_add(const struct Symbol *name);

/* provides size bytes avaliable in text storage */
void text_enlarge(RA_Text size);

/* returns current text address */
RA_Text text_addr(void);

/* adds new operation code to #text */
void text_emit_opcode(char code);

/* emits instruction operand of given type made of constant c */
void text_emit_const(char type, const char *c);

/* emits instruction operand of given type with given name */
void text_emit_symbol(char type, struct Symbol *name);

/* stores label with given name */
void label_add(const struct Symbol *name);

/* adds procedure name to #exp */
void exp_add(const struct Symbol *name);

/* print out debug info about all sections */
void sect_print(void);

/* initializes sections */
void sect_init(void);

/* finishes all sections */
void sect_finish(void);

/* dumps all sections to current output file */
void sect_write(void);

#endif

