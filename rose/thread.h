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
};

void thread_init(Thread *t);

void thread_debug(const Thread *t);

enum ThreadStatus {
	THS_EXIT,
	THS_NOENTRY,
	THS_RUNNING,
	THS_INV_OPCODE,
};

int thread_start(Thread *t, Module *m);

void thread_set_module(Thread *t, Module *m);

#endif

