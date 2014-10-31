/* Working with ROSE module */

#ifndef MODULE_H
#define MODULE_H

#include "rmd.h"
#include "segment.h"
struct Symbol;

/* ROSE module descriptor */
typedef struct Module Module;

struct Segments {
	Exp exp;
	Ptbl ptbl;
	Mtbl mtbl;
	Imp imp;
	Text text;
	Sym sym;
	Str str;
};

struct Module {
	const struct Symbol *name;
	RMDVersion version;
	struct Segments seg;
};

/* minimal supported version */
#define RMD_MIN_VERSION_MAJ   1
#define RMD_MIN_VERSION_MIN   0
/* maximal supported version */
#define RMD_MAX_VERSION_MAJ   1
#define RMD_MAX_VERSION_MIN   0

/* Loads a ROSE module from file 'path'.
 * Returns module handler.
 * On error, returns NULL and sets *errstr to error string */
Module *module_load(const char *path, const char **errstr);

/* Loads a ROSE module with given name.
 * Returns module handler.
 * On error, returns NULL and sets *errstr to error string */
Module *module_get(const struct Symbol *name, const char **errstr);

/* Unloads the module */
/* void module_unload(Module *module); */

#define module_name(m)	((m)->name)

#define module_version(m) (&(m)->version)

int module_find_proc(const Module *m, const struct Symbol *name, RA_Export *proc);

const R_Byte *module_addr_proc(const Module *m, R_Byte idx);

R_Byte module_find_module(const Module *m,
		const struct Symbol *name);

const RMDModule *module_addr_module(const Module *m, R_Byte idx);

#endif

