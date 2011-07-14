#ifndef ROSE_MODULE_H
#define ROSE_MODULE_H

#include "rmd.h"

typedef struct Module Module;

struct ModuleSegment {
	const RMDExport *exp;
	const RMDProcedure *ptbl;
	const RMDModule *mtbl;
	const RMDImport *imp;
	const uint32_t *consts;
	const uint32_t *addr;
	const uint16_t *text;
	const char *sym;
};

struct Module {
	const char *name;
	unsigned char version[2];
	struct ModuleSegment seg;
};

/* minimal supported version */
#define RMD_MIN_VERSION_HI   1
#define RMD_MIN_VERSION_LO   0
/* maximal supported version */
#define RMD_MAX_VERSION_HI   1
#define RMD_MAX_VERSION_LO   0

enum RMDErorr {
	RMD_OK,
	RMD_FILE,
	RMD_READ,
	RMD_NO_MEMORY,
	RMD_BAD_IDENT,
	RMD_BAD_VERSION,
};

Module *module_load(const char *name, int *error);

void module_unload(Module *self);

int verify_ident(const unsigned char ident[4]);

int verify_version(const unsigned char version[2]);

#endif

