#ifndef ROSE_UTILITY_H
#define ROSE_UTILITY_H

typedef struct string {
		const char *data;
		int len;
} string;

#define STR(s)	{ s, sizeof s - 1}

#endif

