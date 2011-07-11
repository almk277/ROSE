#include "rof.h"

#pragma pack(1)

const struct Header header = {
	H_IDENT_INIT,					// signature
	0,								// flags
	0,								// data size
	{ sizeof header +
		sizeof(struct Symbol) * 3, 32 },		// text section
	{ sizeof header, sizeof(struct Symbol) * 3 }			// procedure table
};

const struct Symbol ptable[] = {
	{0, 0 },	// ctor
	{0, 6 },	// outdigit
	{0, 12 },	// newline
};

const unsigned char ctor[] = {
	0x10, 2,		// push 2
	0x10, 3,		// push 3
	0x60, 0,		// add
	0x20, 1,		// call 1
	0x20, 2,		// call 2
	0xFE, 0,		// exit
};

// arg1 - char to output
const unsigned char outdigit[] = {
	0x01, 0,		// prolog
	0x05, -1,		// load -1
	0x10, '0',		// push '0'
	0x60, 0,		// add
	0x0B, 0,		// out 0
	0x22, 0,		// ret
};

const unsigned char newline[] = {
	0, 0,			// prolog
	0x10, '\n',		// push '\n'
	0x0B, 0,		// out 0
	0x22, 0,		// ret
};

