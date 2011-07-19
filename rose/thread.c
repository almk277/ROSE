#include "thread.h"

void thread_debug(const Thread *t)
{
	stack_debug(&t->stack);
}

