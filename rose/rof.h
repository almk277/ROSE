#ifndef ROSE_OBJ_H
#define ROSE_OBJ_H

#include <stdint.h>

struct Section {
	uint32_t offset;
	uint32_t size;
};

struct Symbol {
	uint32_t name;
	uint32_t addr;
};

struct ModuleEntry {
	uint32_t name;
	uint32_t addr;
	struct Section import;
};

struct Header {
	unsigned char ident[4];    /* signature */
	uint32_t flags;            /* various flags */
	uint32_t data_size;        /* data segment size */
	struct Section text;       /* code segment */
	struct Section proc;       /* procedure table struct Address[] */
	struct Section var;        /* variable table struct Address[] */
	struct Section modules;    /* import module table struct ModuleEntry[] */
	struct Section symbols;    /* all symbols as C-strings */
} __attribute__((packed));

/* ROSE signature */
#define H_IDENT1		'R'
#define H_IDENT2		'O'
#define H_IDENT3		'S'
#define H_IDENT4		0x1F
#define H_IDENT_INIT	{ H_IDENT1, H_IDENT2, H_IDENT3, H_IDENT4 }

/* Flags */

#endif

