/* Working with ROSE module */

#ifndef ROSE_MODULE_H
#define ROSE_MODULE_H

#include "segment.h"
#include "disas.h"

/* ROSE module descriptor */
typedef struct Module Module;

struct ModuleSegments {
	Exp exp;
	Ptbl ptbl;
	Mtbl mtbl;
	Imp imp;
	Cnst cnst;
	Addr addr;
	Text text;
	Sym sym;
	Str str;
};

struct Module {
	const char *name;
	unsigned char version[2];
	Module **mtbl;
	struct ModuleSegments seg;
	DisasInfo *info;
};

/* minimal supported version */
#define RMD_MIN_VERSION_HI   1
#define RMD_MIN_VERSION_LO   0
/* maximal supported version */
#define RMD_MAX_VERSION_HI   1
#define RMD_MAX_VERSION_LO   0

/* Errors with ROSE module */
enum RMDError {
	RMD_OK,           /* all right                         */
	RMD_FILE,         /* error while opening file          */
	RMD_READ,         /* error while reading file          */
	RMD_NO_MEMORY,    /* out of memory                     */
	RMD_BAD_IDENT,    /* file has no ROSE signature        */
	RMD_BAD_VERSION,  /* module has an unsupported version */
};

enum ModuleFlags {
	MODULE_NONE    = 0,
	MODULE_DEBUG   = 1,
};

/* Initializes module storage. Must be called before module loading */
void module_init(void);

Module *module_load(const char *name);

/* loads a ROSE module into memory
 * name  - file name;
 * error - place to write error code.
 * returns: pointer to module descriptor, or NULL
 * if an error occurred (error is set in this case */
Module *module_load_file(const char *name, int *error);

/* Unloads a module self */
void module_unload(Module *module);

/* Returns if an array ident contains a correct ROSE signature */
int verify_ident(const unsigned char ident[4]);

/* Returns a given ROSE version is supported */
int verify_rmd_version(const unsigned char version[2]);

/* Looks for a given procedure name in #exp of module m,
 * and returns it's #ptbl index, or -1 if not found */
int module_find_proc(const Module *m, const char *name);

static inline RMDProcedure *module_get_proc(const Module *m, int idx)
{
	return ptbl_get(&m->seg.ptbl, idx);
}

/* For module m, gets module with #mtbl index idx */
Module *module_get_another_module(Module *m, uint8_t idx);

int module_find_external_proc(Module *module, int idx, Module **m);

#endif

