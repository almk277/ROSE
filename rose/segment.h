#ifndef SEGMENT_H
#define SEGMENT_H

#include "rmd.h"

typedef struct Exp {
	const RMDExport *start;
	R_Byte size;
} Exp;

typedef struct Ptbl {
	const RMDProcedure *start;
	R_Byte size;
} Ptbl;

typedef struct Mtbl {
	const RMDModule *start;
	R_Byte size;
} Mtbl;

typedef struct Imp {
	const RMDImport *start;
	R_Byte size;
} Imp;

typedef struct Text {
	const R_Byte *start;
	RA_Text size;
} Text;

typedef struct Sym {
	const char *start;
	RA_Symbol size;
} Sym;

const struct Symbol *sym_get(const Sym *seg, RA_Symbol sym);
#define sym_get(seg, sym)	((const struct Symbol*)&(seg)->start[(sym)])

typedef struct Str {
	const R_Byte *start;
	RA_Array size;
} Str;

#endif

