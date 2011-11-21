#include "stack.h"
#include <glib.h>

static Proc start_proc_frame = {
};

Proc *current_proc = &start_proc_frame;

void stack_init(Stack *stack)
{
	stack->sp = stack->bp = stack->mem;
}

void stack_debug(const Stack *stack)
{
	debug("STACK: mem = %p, sp = %p, bp = %p, top = %d\n",
			stack->mem, stack->sp, stack->bp, stack_top(stack));
}

void stack_new_proc(Stack *stack)
{
	Proc *p = g_slice_new(Proc);
	p->sp = stack->sp;
	p->bp = stack->bp;
	p->prev = current_proc;
	current_proc = p;
}

