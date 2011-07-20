/* [n] pops two stack words and pushes their sum */
static void isa_add(Thread *t)
{
	NOT_IMPLEMENTED("add");
}

/* [p] makes near procedure call */
static void isa_call(Thread *t)
{
	NOT_IMPLEMENTED("call");
}

/* [n] decrements $top */
static void isa_dec(Thread *t)
{
	NOT_IMPLEMENTED("dec");
}

/* [n] deletes object referenced by $top and pops $top */
static void isa_delete(Thread *t)
{
	NOT_IMPLEMENTED("delete");
}

/* [n] stops the program */
static void isa_exit(Thread *t)
{
	t->status =THS_EXIT;
}

/* [f] makes far procedure call */
static void isa_farcall(Thread *t)
{
	NOT_IMPLEMENTED("farcall");
}

/* [a] jumps to given address */
static void isa_farjump(Thread *t)
{
	NOT_IMPLEMENTED("farjump");
}

/* [i] pushes constant on stack */
static void isa_getb(Thread *t)
{
	NOT_IMPLEMENTED("getb");
}

/* [c] pushes constant on stack */
static void isa_getc(Thread *t)
{
	NOT_IMPLEMENTED("getc");
}

/* [d] pushes module variable on stack */
static void isa_getd(Thread *t)
{
	NOT_IMPLEMENTED("getd");
}

/* [s] pushes local variable on stack */
static void isa_gets(Thread *t)
{
	NOT_IMPLEMENTED("gets");
}

/* [n] increments $top */
static void isa_inc(Thread *t)
{
	NOT_IMPLEMENTED("inc");
}

/* [o] jumps to ($ip + $op) */
static void isa_jump(Thread *t)
{
	NOT_IMPLEMENTED("jump");
}

/* [o] pops two stack words and jumps to ($ip + $op) if they are equal */
static void isa_jumpeq(Thread *t)
{
	NOT_IMPLEMENTED("jumpeq");
}

/* [o] pops $top and jumps to ($ip + $op) if $top is less than zero */
static void isa_jumpl(Thread *t)
{
	NOT_IMPLEMENTED("jumpl");
}

/* [o] pops $top and jumps to ($ip + $op) if $top is less or equal to zero */
static void isa_jumple(Thread *t)
{
	NOT_IMPLEMENTED("jumple");
}

/* [o] pops $top and jumps to ($ip + $op) if $top is zero */
static void isa_jumpz(Thread *t)
{
	NOT_IMPLEMENTED("jumpz");
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
static void isa_out(Thread *t)
{
	NOT_IMPLEMENTED("out");
}

/* [d] pops stack word into module variable */
static void isa_putd(Thread *t)
{
	NOT_IMPLEMENTED("putd");
}

/* [s] pops stack word into local variable */
static void isa_puts(Thread *t)
{
	NOT_IMPLEMENTED("puts");
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
	NOT_IMPLEMENTED("sub");
}

