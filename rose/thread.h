#ifndef ROSE_THREAD_H
#define ROSE_THREAD_H

#include "rmd.h"
#include "segment.h"
#include <signal.h>
struct Symbol;
struct Module;

enum ThreadStatus {
	THS_RUNNING = 0,
	THS_EXIT,
	THS_NOENTRY,
	THS_INV_OPCODE,
};

typedef struct ActivRecord {
	struct Module *module;
	R_Byte *retaddr;
	R_Word *varbase;
} ActivRecord;

typedef struct Thread {
	R_Word vstack[2048];
	R_Word *vars;
	ActivRecord pstack[32];
	ActivRecord *procs;
	struct Module *module;
	Text *text;
	const R_Byte *pc;
	//volatile sig_atomic_t status;
	//Proc *current_proc;
} Thread;

void thread_init(Thread *t);

void thread_set_module(Thread *t, struct Module *m);

int thread_jump_to(Thread *t, struct Module *m, const struct Symbol *proc);

int thread_run(Thread *t);

#endif

