/* various RMD tables */

#ifndef SECT_H
#define SECT_H

#include <stdint.h>
#include "cstring.h"

/* adds data with name */
void data_add(const String *name);

/* returns index of data data segment, or -1 if not found */
int data_find(const String *name);

/* adds module with name to #mtbl */
void module_add(const String *name);

/* sets version for last imported module */
void module_set_version(const char *version);

/* returns index of module, or -1 if not found */
int module_find(const String *name);

/* adds extern name from module and with mangled name name to #imp */
void imp_add(const String *name);

/* returns index of extern name, or -1 if not found */
int imp_find(const String *name);

/* sets module name */
void header_set_name(const String *name);

/* sets module version */
void header_set_version(const char *version);

/* sets module parent */
void header_set_parent(const String *name);

/* fills module header with current values */
void header_fill(void);

/* begins new #str string */
void str_begin(const String *name);

/* adds a byte c to current #str string */
void str_add_char(char c);

/* adds a string str to current #str string */
void str_add_string(const String *str);

/* adds procedure name to #ptbl */
void ptbl_add(const String *name);

/* returns procedure index or -1 if not found */
int ptbl_find(const String *name);

/* adds argument with name */
void arg_add(const String *name);

/* adds variable with name */
void var_add(const String *name);

/* returns index of variable, or -1 if not found */
int var_find(const String *name);

/* clears all variables */
void var_clear(void);

/* provides size bytes avaliable in text storage */
void text_enlarge(int size);

uint32_t text_len(void);

/* adds new operation code to #text */
void text_emit_opcode(char code);

/* emits instruction argument of given type with given name */
void text_emit_operand(char type, String *name);

/* stores label with given name */
void label_add(const String *name);

/* clears all labels */
void label_clear(void);

/* adds procedure name to #exp */
void exp_add(const String *name);

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

