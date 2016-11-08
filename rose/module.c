#include "module.h"
/*#include "error.h"*/
#include "mm.h"
#include "serial.h"
#include "symbol.h"
#include "symtbl.h"
#include "loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

MM_DECL(Module, 4);
static SymbolTable module_tbl = SYMBOLTABLE_INITIALIZER;
#define sys_error() (strerror(errno))

static int verify_ident(const unsigned char ident[4])
{
	static const unsigned char correct_ident[] = {
		RMD_H_IDENT1,
		RMD_H_IDENT2,
		RMD_H_IDENT3,
		RMD_H_IDENT4
	};
	return memcmp(ident, correct_ident, sizeof correct_ident) == 0;
}

static int verify_rose_version(RMDVersion ver)
{
	return 1;
}

static Symbol *get_symbol(const Module *m, RA_Symbol idx)
{
	return sym_get(&m->seg, idx);
}

static void add_to_tbl(Module *m, Symbol *name)
{
	SymbolValue *v = symtbl_add_unique(&module_tbl, name);
	v->p = m;
}

static int srd1(FILE *f, unsigned char *buf)
{
	return fread(buf, 1, 1, f) > 0;
}

static int srd2(FILE *f, void *buf)
{
	char b[2];
	int ret;
	if (fread(b, 2, 1, f) == 0)
		return 0;
	ret = b[0] | (b[1] >> 8);
	memcpy(buf, &ret, 2);
	return 1;
}

static int srd4(FILE *f, void *buf)
{
	char b[4];
	long ret;
	if (fread(b, 4, 1, f) == 0)
		return 0;
	ret = b[0] | (b[1] >> 8) | (b[2] >> 16) | (b[3] >> 24);
	memcpy(buf, &ret, 4);
	return 1;
}

/*
static int rd_version(FILE *f, RMDVersion *v)
{
	char buf[2];
	if (fread(buf, 2, 1, f) == 0)
		return -1;
	v->maj = buf[0];
	v->min = buf[1];
	return 0;
}
*/

#define srd(file, addr, size) (srd ## size (file, addr))
#define rd(file, addr, size) (fread(addr, size, 1, file) > 0)

static int read_header(FILE *f, RMDHeader *h)
{
	if (!rd(f, &h->ident, 4)
			|| !srd(f, &h->rmd_version.maj, 1)
			|| !srd(f, &h->rmd_version.min, 1)
			|| !srd(f, &h->name, 2)
			|| !srd(f, &h->version.maj, 1)
			|| !srd(f, &h->version.min, 1)
			|| !srd(f, &h->sizes.exp, 1)
			|| !srd(f, &h->sizes.ptbl, 1)
			|| !srd(f, &h->sizes.mtbl, 1)
			|| !srd(f, &h->sizes.imp, 1)
			|| !srd(f, &h->sizes.text, 4)
			|| !srd(f, &h->sizes.sym, 2)
			|| !srd(f, &h->sizes.str, 4)
			|| !srd(f, &h->datac, 1)
			|| !srd(f, &h->flags, 1)
			|| !srd(f, &h->size, 4)
			|| !rd(f, &h->pad, 8))
		goto err;
	return 0;
err:
	return -1;
}

static Module *module_new(const RMDHeader *h)
{
	Module *m;

	size_t exp_start  = sizeof(Module);
	size_t exp_size   = h->sizes.exp * sizeof(RMDExport);
	size_t ptbl_start = exp_start + exp_size;
	size_t ptbl_size  = h->sizes.ptbl * sizeof(RMDProcedure);
	size_t mtbl_start = ptbl_start + ptbl_size;
	size_t mtbl_size  = h->sizes.mtbl * sizeof(RMDModule);
	size_t imp_start  = mtbl_start + mtbl_size;
	size_t imp_size   = h->sizes.imp * sizeof(RMDImport);
	size_t text_start = imp_start + imp_size;
	size_t text_size  = h->sizes.text;
	size_t sym_start  = text_start + text_size;
	size_t sym_size   = h->sizes.sym;
	size_t str_start  = sym_start + sym_size;
	size_t str_size   = h->sizes.str;

	size_t size = sizeof(Module)
		+ exp_size + ptbl_size + mtbl_size + imp_size
		+ text_size + sym_size + str_size;

	R_Byte *start = malloc(size);
	if (!start)
		return NULL;
	m = (Module *)start;

	m->seg.exp.start  = (RMDExport*)(start + exp_start);
	m->seg.exp.size   = h->sizes.exp;
	m->seg.ptbl.start = (RMDProcedure*)(start + ptbl_start);
	m->seg.ptbl.size  = h->sizes.ptbl;
	m->seg.mtbl.start = (RMDModule*)(start + mtbl_start);
	m->seg.mtbl.size  = h->sizes.mtbl;
	m->seg.imp.start  = (RMDImport*)(start + imp_start);
	m->seg.imp.size   = h->sizes.imp;
	m->seg.text.start = start + text_start;
	m->seg.text.size  = text_size;
	m->seg.sym.start  = (char*)(start + sym_start);
	m->seg.sym.size   = sym_size;
	m->seg.str.start  = start + str_start;
	m->seg.str.size   = str_size;

	m->version = h->version;
	m->datac = h->datac;

	return m;
}

static int read_module(FILE *f, Module *m)
{
	size_t i;

	for (i = 0; i != m->seg.exp.size; ++i) {
		RMDExport e;
		if (!srd(f, &e.name, 2)
				|| !srd(f, &e.idx, 1))
			goto error;
		m->seg.exp.start[i] = e;
	}
	for (i = 0; i != m->seg.ptbl.size; ++i) {
		RMDProcedure p;
		if (!srd(f, &p.addr, 4)
				|| !srd(f, &p.size, 4)
				|| !srd(f, &p.type, 2)
				|| !srd(f, &p.argc, 1)
				|| !srd(f, &p.varc, 1))
			goto error;
		m->seg.ptbl.start[i] = p;
	}
	for (i = 0; i != m->seg.mtbl.size; ++i) {
		RMDModule mod;
		if (!srd(f, &mod.name, 2)
				|| !srd(f, &mod.version.maj, 1)
				|| !srd(f, &mod.version.min, 1))
			goto error;
		m->seg.mtbl.start[i] = mod;
	}
	for (i = 0; i != m->seg.imp.size; ++i) {
		RMDImport imp;
		if (!srd(f, &imp.name, 2)
				|| !srd(f, &imp.type, 2)
				|| !srd(f, &imp.module, 1))
			goto error;
		m->seg.imp.start[i] = imp;
	}
	if (!rd(f, m->seg.text.start, m->seg.text.size)
			|| !rd(f, m->seg.sym.start, m->seg.sym.size)
			|| !rd(f, m->seg.str.start, m->seg.str.size))
		goto error;

	return 0;

error:
	return -1;
}

static Module *load_header(FILE *f, const char **errstr)
{
	RMDHeader h;
	Module *m = NULL;

	if (read_header(f, &h) < 0) {
		*errstr = sys_error();
		goto error;
	}

	if(!verify_ident(h.ident)) {
		*errstr = "no RMD signature";
		goto error;
	}

	if(!verify_rose_version(h.rmd_version)) {
		*errstr = "unsupported ROSE version";
		goto error;
	}

	m = module_new(&h);
	if (!m) {
		*errstr = "no memory";
		goto error;
	}

	if (read_module(f, m) < 0) {
		*errstr = sys_error();
		goto error;
	}

	m->name = get_symbol(m, h.name);
	add_to_tbl(m, m->name);

	return m;

error:
	if (m)
		free(m);
	if(f)
		fclose(f);

	return NULL;
}

Module *module_load(const char *path, const char **errstr)
{
	FILE *f = fopen(path, "rb");
	if(f)
		return load_header(f, errstr);

	*errstr = sys_error();
	return NULL;
}

Module *module_load_obligatory(const char *path)
{
	const char *errstr;
	Module *m = module_load(path, &errstr);
	if(!m) {
		printf("%s: %s\n", path, errstr);
		exit(1);
	}
	return m;
}

Module *module_get(Symbol *name, const char **errstr)
{
	SymbolValue *v;
	FILE *f;

   	v = symtbl_find(&module_tbl, name);
	if(v)
		return v->p;

	f = loader_find_module(name);
	if(f)
		return load_header(f, errstr);

	*errstr = "can not find module";
	return NULL;
}

Module *module_get_obligatory(Symbol *name)
{
	const char *errstr;
	Module *m = module_get(name, &errstr);
	if(!m) {
		fprintf(stderr, "%s: %s\n", name, errstr);
		exit(1);
	}
	return m;
}

Module *module_get_module(Module *m, RA_Module idx)
{
	const Segments *const seg = &m->seg;
	const RMDModule *mod = mtbl_get(seg, idx);
	Symbol *modname = sym_get(seg, mod->name);
	Module *other = module_get_obligatory(modname);
	return other;
}

static int module_exp_find(const Module *m, Symbol *name, RA_Export *proc)
{
	int i;
	const Segments *seg = &m->seg;
	const Exp *exp = &seg->exp;
	Symbol *tryname;
	if (*proc < exp->size) {
		tryname = sym_get(seg, *proc);
		if(strcmp(name, tryname) == 0)
			return 1;
	}
	for(i = 0; i != exp->size; ++i) {
		const RMDExport *e = &exp->start[i];
		tryname = sym_get(seg, e->name);
		if(strcmp(name, tryname) == 0) {
			*proc = i;
			return 1;
		}
	}
	return 0;
}

RA_Export module_exp_get_obligatory(const Module *m,
		Symbol *name, RA_Export hint)
{
	if(!module_exp_find(m, name, &hint)) {
		fprintf(stderr, "%s.%s: procedure not found\n",
				module_name(m), name);
		exit(1);
	}
	return hint;
}

void module_unload(Module *module)
{
	/* TODO remove from table */
	free(module);
}

