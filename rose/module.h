/* ROSE module */

#ifndef MODULE_H
#define MODULE_H

#include "rmd.h"
#include "segment.h"
struct Symbol;

/* ROSE module descriptor */
typedef struct Module Module;

struct Module {
	const struct Symbol *name;
	RMDVersion version;
	Segments seg;
	RA_Data datac;
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

/* Loads a ROSE module from file 'path'.
 * Returns module handler.
 * On error, prints error string and calls 'exit' */
Module *module_load_obligatory(const char *path);

/* Finds or loads a ROSE module with given name.
 * Returns module handler.
 * On error, returns NULL and sets *errstr to error string */
Module *module_get(const struct Symbol *name, const char **errstr);

/* Finds or loads a ROSE module with given name.
 * Returns module handler.
 * On error, prints error string and calls 'exit' */
Module *module_get_obligatory(const struct Symbol *name);

Module *module_get_module(Module *m, RA_Module idx);

/* Unloads the module */
/* void module_unload(Module *module); */

/* returns name of the module m */
#define module_name(m)	((m)->name)

/* returns version of the module m */
#define module_version(m) (&(m)->version)

#define module_data_size(m) ((m)->datac)

/* Finds in module m procedure with given name and supposed index hint.
 * Returns index in #exp.
 * If not found, prints error string and exits */
RA_Export module_exp_get_obligatory(const Module *m,
		const struct Symbol *name, RA_Export hint);

#endif

