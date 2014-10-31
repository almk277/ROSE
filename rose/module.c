#include "module.h"
/*#include "error.h"*/
#include "mm.h"
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

static const Symbol *get_symbol(const Module *m, RA_Symbol idx)
{
	return (Symbol*)&m->seg.sym.start[idx];
}

static void module_set(Module *m, const RMDHeader *h, const char *addr)
{
	struct Segments *seg = &m->seg;

#define SET_SEGMENT(name, prevname) \
	{ \
		seg->name.size = h->sizes.name; \
		seg->name.start = (void*)(addr += \
				h->sizes.prevname * sizeof(*seg->prevname.start)); \
	}
	seg->exp.size = h->sizes.exp;
	seg->exp.start = (void*)addr;
	SET_SEGMENT(ptbl, exp);
	SET_SEGMENT(mtbl, ptbl);
	SET_SEGMENT(imp, mtbl);
	SET_SEGMENT(text, imp);
	SET_SEGMENT(sym, text);
	SET_SEGMENT(str, sym);
#undef SET_SEGMENT

	m->name = get_symbol(m, h->name);
	m->version = h->version;
}

static void add_to_tbl(Module *m, const Symbol *name)
{
	SymbolValue *v = symtbl_add_unique(&module_tbl, name);
	v->p = m;
}

static Module *module_read(const RMDHeader *h, FILE *f, const char **errstr)
{
	char *content;
	size_t size = sizeof(Module)            /* header */
		+ h->size                           /* sectors */
		+ h->sizes.mtbl * sizeof(Module*);  /* procedure cache */

	Module *m = malloc(size);
	if(!m) {
		*errstr = "no memory";
		goto clean;
	}

	content = (char *)m + sizeof(Module);
	if(fread(content, h->size, 1, f) != 1) {
		*errstr = sys_error();
		goto clean;
	}

	module_set(m, h, content);
	add_to_tbl(m, m->name);
	return m;
clean:
	free(m);
	return NULL;
}

static Module *load_header(FILE *f, const char **errstr)
{
	RMDHeader h;
	Module *m = NULL;

	if(fread(&h, sizeof h, 1, f) != 1) {
		*errstr = sys_error();
		goto clean;
	}

	if(!verify_ident(h.ident)) {
		*errstr = "no RMD signature";
		goto clean;
	}

	if(!verify_rose_version(h.rmd_version)) {
		*errstr = "unsupported ROSE version";
		goto clean;
	}

	m = module_read(&h, f, errstr);
clean:
	if(f)
		fclose(f);
	return m;
}

Module *module_load(const char *path, const char **errstr)
{
	FILE *f = fopen(path, "rb");
	if(!f) {
		*errstr = sys_error();
		return NULL;
	}
	return load_header(f, errstr);
}

Module *module_get(const Symbol *name, const char **errstr)
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

int module_find_proc(const Module *m, const Symbol *name, RA_Export *proc)
{
	int i;
	const Exp *exp = &m->seg.exp;
	const Sym *sym = &m->seg.sym;
	const Symbol *tryname = sym_get(sym, *proc);
	if(symbol_compare(name, tryname))
		return 1;
	for(i = 0; i != exp->size; ++i) {
		const RMDExport *e = &exp->start[i];
		tryname = sym_get(sym, e->name);
		if(symbol_compare(name, tryname)) {
			*proc = i;
			return 1;
		}
	}
	return 0;
}

#if 0
void module_unload(Module *module)
{
	/* remove from table */
	free(module);
}

static inline void *section_address(const Module *module, uint32_t offset)
{
	return (char *)module + sizeof(Module) - sizeof(RMDHeader) + offset;
}

static Module *create_module(RMDHeader *h, FILE *f, int *error)
{
	module->mtbl = (Module **)((char *)module + sizeof(Module) + h->size);
	memset(module->mtbl, 0, h->mtbl * sizeof(Module *));
	return module;
}

int module_find_external_proc(Module *module, int idx, Module **m)
{
	int ptbl_idx;
	RMDImport *imp = imp_get(&module->seg.imp, idx);
	const char *name = sym_get(&module->seg.sym, imp->name);
	*m = module_get_another_module(module, imp->module);
	ptbl_idx = module_find_proc(*m, name);
	if(ptbl_idx == -1)
		/* TODO: output procedure name */
		error(ERR_NO_PROC);
	return ptbl_idx;
}

Module *module_get_another_module(Module *module, uint8_t idx)
{
	Module *m = module->mtbl[idx];
	/* if module was referenced from here - it is cached */
	if(!m) {
		/* module is either not referenced from this module,
		 * or not loaded at all */
		uint16_t name_ofs = mtbl_get_name(&module->seg.mtbl, idx);
		const char *name = sym_get(&module->seg.sym, name_ofs);
		m = module_find(name);
		if(!m)
			m = module_load(name);
		/* anyway, cache it */
		module->mtbl[idx] = m;
	}
	return m;
}
#endif

