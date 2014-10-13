/* various RMD tables */

#ifndef SECT_H
#define SECT_H

#include <stdint.h>
#include "symbol.h"

/* adds data with name */
void data_add(const Symbol *name);

/* adds module with name to #mtbl */
void module_add(const Symbol *name);

/* sets version for last imported module */
void module_set_version(const char *version);

/* adds extern name to #imp */
void imp_add(const Symbol *name);

/* sets module name */
void header_set_name(const Symbol *name);

/* sets module version */
void header_set_version(const char *version);

/* sets module parent */
void header_set_parent(const Symbol *name);

/* fills module header with current values */
void header_fill(void);

/* begins new #str string */
void str_begin(const Symbol *name);

/* adds a byte c to current #str string */
void str_add_char(char c);

/* adds procedure name to #ptbl */
void ptbl_add(const Symbol *name);

/* adds argument with name */
void arg_add(const Symbol *name);

/* adds variable with name */
void var_add(const Symbol *name);

/* provides size bytes avaliable in text storage */
void text_enlarge(int size);

uint32_t text_len(void);

/* adds new operation code to #text */
void text_emit_opcode(char code);

/* emits instruction operand of given type made of constant c */
void text_emit_const(char type, const char *c);

/* emits instruction operand of given type with given name */
void text_emit_symbol(char type, Symbol *name);

/* stores label with given name */
void label_add(const Symbol *name);

/* adds procedure name to #exp */
void exp_add(const Symbol *name);

/* print out debug info about all sections */
#ifdef DEBUG
void sect_print(void);
#else
#define sect_print()
#endif

/* initializes sections */
void sect_init(void);

/* finishes all sections */
void sect_finish(void);

/* dumps all sections to current output file */
void sect_write(void);

#endif

