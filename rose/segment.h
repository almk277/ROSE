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

typedef struct Str {
	const R_Byte *start;
	RA_Array size;
} Str;

typedef struct Segments {
	Exp exp;
	Ptbl ptbl;
	Mtbl mtbl;
	Imp imp;
	Text text;
	Sym sym;
	Str str;
} Segments;

/* const RMDExport *exp_get(const Segments *seg, RA_Export i); */
#define exp_get(seg, i)  (&(seg)->exp.start[i])

/* const RMDProcedure *proc_get(const Segments *seg, RA_Procecure i); */
#define ptbl_get(seg, i) (&(seg)->ptbl.start[i])

/* const RMDModule *mtbl_get(const Segments *seg, RA_Module i); */
#define mtbl_get(seg, i) (&(seg)->mtbl.start[i])

/* const RMDImport *imp_get(const Segments *seg, RA_Import i); */
#define imp_get(seg, i)  (&(seg)->imp.start[i])

/* const R_Byte *text_get(const Segments *seg, RA_Text i); */
#define text_get(seg, i) (&(seg)->text.start[i])

/* const struct Symbol *sym_get(const Segments *seg, RA_Symbol i); */
#define sym_get(seg, i)	 ((const struct Symbol*)&(seg)->sym.start[i])

#endif

