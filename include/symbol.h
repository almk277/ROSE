#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdio.h>

/* Symbol is a short atomic string */
typedef struct Symbol Symbol;

/* Type of symbol length */
typedef unsigned char SymbolSize;

/* Creates new symbol of len size and copies str into it */
Symbol *symbol_new(const char *str, int len);

/* Deletes symbol s */
void symbol_delete(Symbol *s);

/* Returns length of symbol s */
SymbolSize symbol_length(const Symbol *s);

/* Compares symbols s1 and s2, returns 1 if the are equal, 0 otherwise */
int symbol_compare(const Symbol *s1, const Symbol *s2);

/* Returns hash byte for symbol s */
unsigned char symbol_hash(const Symbol *s);

/* Prints symbol to standard output */
void symbol_print(const Symbol *s);

/* Prints symbol to file */
void symbol_print_to_file(const Symbol *s, FILE *file);

/* Copies symbol to given buffer */
void symbol_copy_to(const Symbol *s, char *buffer);

/* Splits up s at ":" position into symbols s1 and s2.
 * Returns 1 if success (there is ":"), 0 otherwise */
int symbol_split_colon(const Symbol *s, Symbol **s1, Symbol **s2);

#endif

