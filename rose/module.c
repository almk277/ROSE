#include "module.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

static GHashTable *module_tbl;

/* returns if maj1.min1 is greater than maj2.min2 */
#define VERSION_GREATER(maj1, min1, maj2, min2) \
	(maj1 > maj2 || (maj1 == maj2 && min1 >= min2))
/* returns if maj1.min1 is less than maj2.min2 */
#define VERSION_LESS(maj1, min1, maj2, min2) \
	(maj1 < maj2 || (maj1 == maj2 && min1 <= min2))

void module_init(void)
{
	module_tbl = g_hash_table_new(g_str_hash, g_str_equal);
}

static inline void module_to_hash(Module *module)
{
	/* FIXME why not const void *? */
	g_hash_table_insert(module_tbl, (char *)module->name, module);
}

static inline Module *module_find(const char *name)
{
	return g_hash_table_lookup(module_tbl, name);
}

Module *module_load(const char *name)
{
	int err;
	Module *m = module_load_file(name, &err);
	if(!m)
		error(ERR_NO_MODULE);
	return m;
}

int verify_ident(const unsigned char ident[4])
{
	static const unsigned char correct_ident[] = {
		RMD_H_IDENT1,
		RMD_H_IDENT2,
		RMD_H_IDENT3,
		RMD_H_IDENT4
	};
	return memcmp(ident, correct_ident, sizeof correct_ident) == 0;
}

int verify_rmd_version(const unsigned char version[2])
{
	/* hack to get rid of gcc warning */
	const int min_version_lo = RMD_MIN_VERSION_LO;
	return VERSION_GREATER(version[0], version[1],
			RMD_MIN_VERSION_HI, min_version_lo)
		&& VERSION_LESS(version[0], version[1],
				RMD_MAX_VERSION_HI, RMD_MAX_VERSION_LO);
}

static Module *create_module(RMDHeader *h, FILE *f, int *error);

Module *module_load_file(const char *name, int *error)
{
	RMDHeader h;
	Module *module = 0;
	FILE *f;

   	f = fopen(name, "r");
	if(!f) {
		*error = RMD_FILE;
		return NULL;
	}
	if(fread(&h, sizeof h, 1, f) != 1) {
		*error = RMD_READ;
		goto close;
	}
	if(!verify_ident(h.ident)) {
		*error = RMD_BAD_IDENT;
		goto close;
	}
	if(!verify_rmd_version(h.rmd_ver)) {
		*error = RMD_BAD_VERSION;
		goto close;
	}

	module = create_module(&h, f, error);
close:
	fclose(f);
	return module;
}

static inline void *section_address(const Module *module, uint32_t offset)
{
	return (char *)module + sizeof(Module) - sizeof(RMDHeader) + offset;
}

static Module *create_module(RMDHeader *h, FILE *f, int *error)
{
	char *start;
	struct ModuleSegments *seg;
	Module *module = malloc(sizeof(Module)
			+ h->size
			+ h->mtbl * sizeof(Module *)
			);
	if(!module) {
		*error = RMD_NO_MEMORY;
		return NULL;
	}
	start = (char *)module + sizeof(Module);
	if(fread(start, h->size, 1, f) != 1) {
		*error = RMD_READ;
		free(module);
		return NULL;
	}
	seg = &module->seg;

#define SET_SIZE(name) seg->name.size = h->name
	SET_SIZE(exp);
	SET_SIZE(ptbl);
	SET_SIZE(mtbl);
	SET_SIZE(imp);
	SET_SIZE(cnst);
	SET_SIZE(addr);
	SET_SIZE(text);
	SET_SIZE(sym);
	SET_SIZE(str);

#define SET_START(segment, type, prev, prevtype) \
	seg->segment.start = (type *)(start += h->prev * sizeof(prevtype))

	seg->exp.start  = (RMDExport *)start;
	SET_START(ptbl, RMDProcedure, exp, RMDExport);
	SET_START(mtbl, RMDModule, ptbl, RMDProcedure);
	SET_START(imp, RMDImport, mtbl, RMDModule);
	SET_START(cnst, int32_t, imp, RMDImport);
	SET_START(addr, uint32_t, cnst, int32_t);
	SET_START(text, char, addr, uint32_t);
	SET_START(sym, char, text, char);
	SET_START(str, char, sym, char);

	module->mtbl = (Module **)(module + sizeof(Module) + h->size);
	memset(module->mtbl, 0, h->mtbl * sizeof(Module *));
	module->name = sym_get(&module->seg.sym, h->name);
	module->version[0] = h->version[0];
	module->version[1] = h->version[1];

	module_to_hash(module);
	return module;
}

void module_unload(Module *module)
{
	free(module);
}

int module_find_proc(const Module *m, const char *name)
{
	int i;
	for(i = 0; i != m->seg.exp.size; ++i) {
		RMDExport *e = exp_get(&m->seg.exp, i);
		if(strcmp(sym_get(&m->seg.sym, e->name), name) == 0)
			return i;
	}
	return -1;
}

uint32_t module_proc_addr(const Module *m, int idx)
{
	return ptbl_get(&m->seg.ptbl, idx)->addr;
}

Module *module_get_dependent(Module *module, uint8_t idx)
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

