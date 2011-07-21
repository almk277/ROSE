#ifndef ROSE_STACK_H
#define ROSE_STACK_H

#include "debug.h"
#include <stdint.h>

#define STACK_SIZE   1024

typedef struct Stack Stack;

struct Stack {
	int32_t mem[STACK_SIZE];
	int32_t *sp;
	int32_t *bp;
};

void stack_init(Stack *stack);

static inline void stack_push(Stack *stack, int32_t elem)  
{
	*++stack->sp = elem;
}

static inline int32_t stack_pop(Stack *stack)
{
	return *(stack--)->sp;
}

static inline int32_t stack_at(const Stack *stack, int ofs)
{
	return stack->bp[ofs];
}

static inline int32_t *stack_at_p(const Stack *stack, int ofs)
{
	return &stack->bp[ofs];
}

static inline int32_t stack_top(const Stack *stack)
{
	return *stack->sp;
}

static inline int32_t *stack_top_p(const Stack *stack)
{
	return stack->sp;
}

void stack_debug(const Stack *stack);

#endif

