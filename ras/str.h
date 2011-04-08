/* working with strings */

#ifndef STRPOOL_H
#define STRPOOL_H

#include <stddef.h>

/* creates copy for C-string str */
char *str_clone(const char *str);

/* frees memory allocated for str with str_close */
void str_free(char *str);

#endif

