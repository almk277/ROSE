#ifndef ROSE_THREAD_H
#define ROSE_THREAD_H

#include "stack.h"
#include "module.h"

typedef struct Thread Thread;

struct Thread {
	Stack stack;
	Stack proc;
	Module *context;
};

void thread_debug(const Thread *t);

#endif

