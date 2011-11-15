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

/* #const element count */
uint8_t const_count(void);

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
uint16_t sym_count(void);

/* adds module with name to #mtbl */
void module_add(const char *name, uint8_t maj, uint8_t min);

/* returns index of module, or -1 if not found */
int module_find(const char *name);

/* debug output for #mtbl */
void module_print(void);

/* dumps #mtbl to file */
void module_write(FILE *file);

/* #mtbl element count */
uint8_t module_count(void);

/* adds extern name from module and with mangled name mangl_name to #imp */
RMDImport *imp_add(char *mangl_name);

/* returns index of extern name, or -1 if not found */
int imp_find(const char *mangl_name);

/* debug output for #imp */
void imp_print(void);

/* dumps #imp to file */
void imp_write(FILE *file);

/* #imp element count */
uint8_t imp_count(void);

/* module header */
extern RMDHeader header;

/* debug print for header */
void header_print(void);

/* dumps header to file */
void header_write(FILE *file);

/* adds new string with given name and returns address of it's length */
uint32_t *str_add_string(const char *name);

/* adds a byte c to #str, str_len points to current string length */
void str_add_char(uint32_t *str_len, char c);

/* #str length */
uint32_t str_count(void);

/* returns address of a string with given name, or (uint32_t)-1 if not found */
uint32_t str_find(const char *name);

/* debug output for #str */
void str_print(void);

/* dumps #str to file */
void str_write(FILE *file);

#endif

