/* various RMD tables */

#ifndef TABLES_H
#define TABLES_H

#include <stdint.h>
#include <stdio.h>
#include "rmd.h"

/* adds constant with name and value val to #const */
void const_add(const char *name, int32_t val);

/* returns index of constant in #const, or -1 if not found */
int const_find(const char *name);

/* debug print for #const */
void const_print(void);

/* dumps #const in file */
void const_write(FILE *file);

/* #const length */
size_t const_length(void);

/* adds data with name */
void data_add(const char *name);

/* returns index of data data segment, or -1 if not found */
int data_find(const char *name);

/* debug print for data segment */
void data_print(void);

/* count of data items */
uint8_t data_count(void);

/* adds name to #sym and returns it's address */
uint16_t sym_add(const char *name);

/* debug print for #sym */
void sym_print(void);

/* dumps #sym in file */
void sym_write(FILE *file);

/* #sym length */
size_t sym_length(void);

/* adds variable with name */
void var_add(const char *name);

/* returns index of variable, or -1 if not found */
int var_find(const char *name);

/* clears all variables */
void var_clear(void);

/* debug output for variables */
void var_print(void);

/* adds module with name to #mtbl */
void module_add(const char *name, uint8_t maj, uint8_t min);

/* returns index of module, or -1 if not found */
int module_find(const char *name);

/* debug output for #mtbl */
void module_print(void);

/* dumps #mtbl to file */
void module_write(FILE *file);

/* #mtbl length */
size_t module_length(void);

/* adds extern name from module and with mangled name mangl_name to #imp */
RMDImport *imp_add(char *mangl_name);

/* returns index of extern name, or -1 if not found */
int imp_find(const char *mangl_name);

/* debug output for #imp */
void imp_print(void);

/* dumps #imp to file */
void imp_write(FILE *file);

/* #imp length */
size_t imp_length(void);

/* module header */
extern RMDHeader header;

/* debug print for header */
void header_print(void);

/* dumps header to file */
void header_write(FILE *file);

#endif

