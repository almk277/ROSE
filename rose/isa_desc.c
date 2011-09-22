/* [n] pops two stack words and pushes their sum */
static void isa_add(Thread *t)
{
	int32_t a2 = stack_pop(ST);
	int32_t a1 = stack_pop(ST);
	/* FIXME overflow */
	int32_t a = a1 + a2;
	stack_push(ST, a);
}

/* [p] makes near procedure call */
static void isa_call(Thread *t)
{
	NOT_IMPLEMENTED("call");
}

/* [n] decrements $top */
static void isa_dec(Thread *t)
{
	*stack_top_p(ST) = stack_top(ST) - OP;
}

/* [n] deletes object referenced by $top and pops $top */
static void isa_delete(Thread *t)
{
	NOT_IMPLEMENTED("delete");
}

/* [n] stops the program */
static void isa_exit(Thread *t)
{
	t->status = THS_EXIT;
}

/* [f] makes far procedure call */
static void isa_farcall(Thread *t)
{
	NOT_IMPLEMENTED("farcall");
}

/* [a] jumps to given address */
static void isa_farjump(Thread *t)
{
	uint32_t addr = addr_get(&t->module->seg.addr, OP);
	text_goto(TX, addr);
}

/* [c] pushes constant on stack */
static void isa_getc(Thread *t)
{
	int32_t c = cnst_get(&t->module->seg.cnst, OP);
	stack_push(ST, c);
}

/* [d] pushes module variable on stack */
static void isa_getd(Thread *t)
{
	NOT_IMPLEMENTED("getd");
}

/* [s] pushes local variable on stack */
static void isa_gets(Thread *t)
{
	int32_t a = stack_at(ST, OP);
	stack_push(ST, a);
}

/* [n] increments $top */
static void isa_inc(Thread *t)
{
	*stack_top_p(ST) = stack_top(ST) + OP;
}

/* [o] jumps to ($ip + $op) */
static void isa_jump(Thread *t)
{
	text_jump_ofs(TX, OP);
}

/* [o] pops two stack words and jumps to ($ip + $op) if they are equal */
static void isa_jumpeq(Thread *t)
{
	int32_t a2 = stack_pop(ST);
	int32_t a1 = stack_pop(ST);
	if(a1 == a2)
		text_jump_ofs(TX, OP);
}

/* [o] pops $top and jumps to ($ip + $op) if $top is less than zero */
static void isa_jumpl(Thread *t)
{
	int32_t a = stack_pop(ST);
	if(a < 0)
		text_jump_ofs(TX, OP);
}

/* [o] pops $top and jumps to ($ip + $op) if $top is less or equal to zero */
static void isa_jumple(Thread *t)
{
	int32_t a = stack_pop(ST);
	if(a <= 0)
		text_jump_ofs(TX, OP);
}

/* [o] pops $top and jumps to ($ip + $op) if $top is zero */
static void isa_jumpz(Thread *t)
{
	int32_t a = stack_pop(ST);
	if(a == 0)
		text_jump_ofs(TX, OP);
}

/* [c] pushes reference to array with address $op */
static void isa_loadstr(Thread *t)
{
	/* FIXME reference can not be freed */
	uint32_t ofs = stack_top(ST);
	void *addr = str_addr(&t->module->seg.str, ofs);
	Ref ref = ref_new(addr);
	*stack_top_p(ST) = ref_to(ref);
}

/* [m] creates new module object and pushes reference to it on stack */
static void isa_new(Thread *t)
{
	NOT_IMPLEMENTED("new");
}

/* [n] nop does nothing */
static void isa_nop(Thread *t)
{
	(void)t;
}

/* [u] pops stack word and outputs it in given file */
/* FIXME This is broken design */
static void isa_out(Thread *t)
{
	int32_t a = stack_pop(ST);
	int8_t b = a & 0xFF;
	putchar(b);
}

/* [i] pushes constant on stack */
static void isa_pushc(Thread *t)
{
	stack_push(ST, OP);
}

/* [i] makes $top = ($top << 8) | $op */
static void isa_pushcsh(Thread *t)
{
	int32_t top = stack_top(ST);
	top = (top << 8) | OP;
	*stack_top_p(ST) = top;
}

/* [d] pops stack word into module variable */
static void isa_putd(Thread *t)
{
	NOT_IMPLEMENTED("putd");
}

/* [s] pops stack word into local variable */
static void isa_puts(Thread *t)
{
	int32_t a = stack_pop(ST);
	*stack_at_p(ST, OP) = a;
}

/* [n] returns execution from current procedure with return value */
static void isa_return(Thread *t)
{
	NOT_IMPLEMENTED("return");
}

/* [n] returns execution from current procedure without return value */
static void isa_retp(Thread *t)
{
	NOT_IMPLEMENTED("retp");
}

/* [n] pops two stack words and pushes their difference */
static void isa_sub(Thread *t)
{
	int32_t a2 = stack_pop(ST);
	int32_t a1 = stack_pop(ST);
	/* FIXME overflow */
	int32_t a  = a1 - a2;
	stack_push(ST, a);
}

/* [n] gets array length: $top = length($top) */
static void isa_strlen(Thread *t)
{
	Ref ref = ref_from(stack_top(ST));
	uint32_t *len = ref_get(ref);
	*stack_top_p(ST) = *len;
}

/* [n] pushes $top on stack again */
static void isa_dup(Thread *t)
{
	stack_push(ST, stack_top(ST));
}

/* [u] writes to given file array $top bytes from array $top[-1] */
static void isa_write(Thread *t)
{
	char *start;
	uint32_t i;
	uint32_t len = stack_pop(ST);
	Ref ref = ref_from(stack_pop(ST));
	uint32_t *array_len = ref_get(ref);
	if(len > *array_len)
		len = *array_len;
	/* pass array length */
	start = (char *)(array_len + 1);
	for(i = 0; i != len; ++i)
		putchar(start[i]);
}

