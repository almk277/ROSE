#ifndef ROSE_TEXT_H
#define ROSE_TEXT_H

#include "rmd.h"
#include <stdint.h>
#include <stddef.h>

typedef struct Text Text;

struct Text {
	unsigned char *start;
	uint32_t size;
	const uint8_t *pc;
};

void text_init(Text *text, char *start, const RMDHeader *h);

static inline uint8_t *text_addr(const Text *text, uint32_t ofs)
{
	return &text->start[ofs];
}

static inline uint8_t text_fetch(Text *text)
{
	return *text->pc++;
}

static inline void text_goto(Text *text, uint8_t *addr)
{
	text->pc = addr;
}

#endif

