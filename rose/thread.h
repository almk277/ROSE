#ifndef ROSE_THREAD_H
#define ROSE_THREAD_H

#include "stack.h"
#include "module.h"

typedef struct Thread Thread;

struct Thread {
	Stack stack;
	Stack proc;
	Module *module;
	Text *text;
	int status;
	uint8_t operand;
	Proc *current_proc;
};

void thread_init(Thread *t);

void thread_debug(const Thread *t);

enum ThreadStatus {
	THS_RUNNING = 0,
	THS_EXIT,
	THS_NOENTRY,
	THS_INV_OPCODE,
};

int thread_start(Thread *t, Module *m);

void thread_set_module(Thread *t, Module *m);

void thread_proc_call(Thread *t, Module *m, uint8_t idx);

void thread_proc_ret(Thread *t, int retval);

void thread_debug_start(Thread *t, Module *m);

#endif

