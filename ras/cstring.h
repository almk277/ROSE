#ifndef CSTRING_H
#define CSTRING_H

#include <stdint.h>

/* Just a string */
typedef struct String {
	uint32_t len;
	char data[1];
} String;

/* Creates new string of len size and copies str into it */
String *string_new(const char *str, int len);

/* Deletes string s */
void string_delete(String *s);

#endif

