#ifndef ROSE_THREAD_H
#define ROSE_THREAD_H

#include "rmd.h"
#include "symbol.h"
#include "segment.h"
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
	union {
		const R_Byte        *byte;
		const R_Word        *word;
		const RA_TextOffset *ofs;
		const RA_Stack      *stack;
		const RA_Data       *data;
		const RA_Module     *module;
		const RA_Proc       *proc;
		const RA_Import     *import;
		const RA_Symbol     *symbol;
		const RA_Array      *array;
	} pc;                            /* instruction pointer */
} Thread;

int thread_init(Thread *t, struct Module *m, Symbol *proc);

void thread_run(Thread *t);

int thread_call_intern(Thread *t, struct Module *module, RA_Proc p_idx);

int thread_call_extern(Thread *t, struct Module *module, RA_Import p_idx);

void thread_return(Thread *t);

#endif

