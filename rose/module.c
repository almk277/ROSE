#include "module.h"
#include "ptbl.h"
#include <stdio.h>
#include <stdlib.h>

/* returns if maj1.min1 is greater than maj2.min2 */
#define VERSION_GREATER(maj1, min1, maj2, min2) \
	(maj1 > maj2 || (maj1 == maj2 && min1 >= min2))
/* returns if maj1.min1 is less than maj2.min2 */
#define VERSION_LESS(maj1, min1, maj2, min2) \
	(maj1 < maj2 || (maj1 == maj2 && min1 <= min2))

#define SEG_INIT ptbl_init(module->seg.ptbl, ofs_start + h->exp, (h->mtbl - h->ptbl));

static size_t load_size(const RMDHeader *header)
{
	return header->str - sizeof(RMDHeader);
}

int verify_ident(const unsigned char ident[4])
{
	static const unsigned char correct_ident[] = {
		RMD_H_IDENT1,
		RMD_H_IDENT2,
		RMD_H_IDENT3,
		RMD_H_IDENT4
	};
	return memcmp(ident, correct_ident, sizeof ident) == 0;
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

Module *module_load(const char *name, int *error)
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

static void *section_address(const Module *module, uint32_t offset)
{
	return (char *)module + sizeof(Module) - sizeof(RMDHeader) + offset;
}

static Module *create_module(RMDHeader *h, FILE *f, int *error)
{
	char *start, *ofs_start;
	const size_t size = load_size(h);
	Module *module = malloc(sizeof(Module) + size);
	if(!module) {
		*error = RMD_NO_MEMORY;
		return NULL;
	}
	start = (char *)module + sizeof(Module);
	if(fread(start, size, 1, f) != 1) {
		*error = RMD_READ;
		free(module);
		return NULL;
	}
	ofs_start = start - sizeof(RMDHeader);

	exp_init(&module->seg.exp, ofs_start, h);
	ptbl_init(&module->seg.ptbl, ofs_start, h);
	module->seg.mtbl   = section_address(module, h->mtbl);
	module->seg.imp    = section_address(module, h->imp);
	module->seg.consts = section_address(module, h->consts);
	module->seg.addr   = section_address(module, h->addr);
	text_init(&module->seg.text, ofs_start, h);
	sym_init(&module->seg.sym, ofs_start, h);

	module->name = sym_get(&module->seg.sym, h->name);
	module->version[0] = h->version[0];
	module->version[1] = h->version[1];

	return module;
}

void module_unload(Module *self)
{
	free(self);
}

const int module_find_proc(const Module *m, const char *name)
{
	int i;
	for(i = 0; i != m->seg.exp.size; ++i) {
		RMDExport *e = exp_get(&m->seg.exp, i);
		if(strcmp(sym_get(&m->seg.sym, e->name), name) == 0)
			return i;
	}
	return -1;
}

