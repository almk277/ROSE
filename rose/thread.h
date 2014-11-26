#ifndef ROSE_THREAD_H
#define ROSE_THREAD_H

#include "rmd.h"
#include "segment.h"
struct Symbol;
struct Module;

/* activation record */
typedef struct ActivRecord {
	struct Module *module;           /* module */
	const RMDProcedure *proc;        /* procedure */
	const R_Byte *retaddr;           /* return address */
	R_Word *varbase;                 /* variable frame start */
} ActivRecord;

/* thread descriptor */
typedef struct Thread {
	R_Word vstack[2048];             /* variable stack */
	R_Word *vars, *vars_end;         /* current and end variable frames */
	ActivRecord pstack[32];          /* activation record stack */
	ActivRecord *procs, *procs_end;  /* current and end record frames */
	struct Module *module;           /* current module */
	union {
		const R_Byte *byte;
		const RA_TextOffset *ofs;
		const R_Word *word;
	} pc;                            /* instruction pointer */
} Thread;

int thread_init(Thread *t, struct Module *m, const struct Symbol *proc);

void thread_run(Thread *t);

int thread_call_intern(Thread *t, struct Module *module, RA_Proc p_idx);

int thread_call_extern(Thread *t, struct Module *m, RA_Export);

void thread_return(Thread *t);

#endif

