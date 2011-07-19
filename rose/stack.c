#include "stack.h"

void stack_init(Stack *stack)
{
	stack->sp = stack->bp = stack->mem;
}

void stack_debug(const Stack *stack)
{
	debug("STACK: mem = %p, sp = %p, bp = %p, top = %d\n",
			stack->mem, stack->sp, stack->bp, stack_top(stack));
}

