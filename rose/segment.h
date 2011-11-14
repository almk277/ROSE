#ifndef ROSE_SEGMENT_H
#define ROSE_SEGMENT_H

#include "rmd.h"

/*************************  .exp  ************************/

typedef struct Exp Exp;

struct Exp {
	RMDExport *start;
	uint8_t size;
};

static inline RMDExport *exp_get(const Exp *exp, int idx)
{
	return &exp->start[idx];
}

static inline uint8_t exp_idx_get(const Exp *exp, int idx)
{
	return exp_get(exp, idx)->idx;
}

/*************************  .ptbl  ************************/

typedef struct Ptbl Ptbl;

struct Ptbl {
	RMDProcedure *start;
	uint8_t size;
};

static inline RMDProcedure *ptbl_get(const Ptbl *ptbl, int idx)
{
	return &ptbl->start[idx];
}

/*************************  .mtbl  ************************/

typedef struct Mtbl Mtbl;
struct Module;

struct Mtbl {
	RMDModule *start;
	uint8_t size;
};

static inline uint16_t mtbl_get_name(const Mtbl *mtbl, uint8_t idx)
{
	return mtbl->start[idx].name;
}

/*************************  .imp  ************************/

typedef struct Imp Imp;

struct Imp {
	RMDImport *start;
	uint8_t size;
};

/*************************  .cnst  ************************/

typedef struct Cnst Cnst;

struct Cnst {
	int32_t *start;
	uint8_t size;
};

static inline int32_t cnst_get(const Cnst *cnst, int idx)
{
	return cnst->start[idx];
}

void cnst_debug(Cnst *cnst);

/*************************  .addr  ************************/

typedef struct Addr Addr;

struct Addr {
	uint32_t *start;
	uint8_t size;
};

static inline uint32_t addr_get(const Addr *addr, int idx)
{
	return addr->start[idx];
}

void addr_debug(const Addr *addr);

/*************************  .text  ************************/

typedef struct Text Text;

struct Text {
	unsigned char *start;
	uint32_t size;
	const uint8_t *pc;
};

static inline uint8_t *text_addr(const Text *text, uint32_t ofs)
{
	return &text->start[ofs];
}

static inline uint8_t text_fetch(Text *text)
{
	return *text->pc++;
}

static inline void text_goto(Text *text, uint32_t addr)
{
	text->pc = text->start + addr;
}

static inline void text_jump_ofs(Text *text, uint8_t ofs)
{
	int8_t _ofs = (int8_t)ofs;
	text->pc += 2 * _ofs;
}

typedef struct Sym Sym;

/*************************  .sym  ************************/

struct Sym {
	const char *start;
	uint16_t size;
};

static inline const char *sym_get(const Sym *sym, uint16_t ofs)
{
	return &sym->start[ofs];
}

/*************************  .str  ************************/

typedef struct Str Str;

struct Str {
	char *start;
	uint32_t size;
};

static inline void *str_addr(Str *str, uint32_t offset)
{
	return str->start + offset;
}

#endif

