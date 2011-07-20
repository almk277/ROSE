/* pops two stack words and pushes their sum */
static void isa_add(Thread *t)
{
	NOT_IMPLEMENTED("add");
}

/* makes near procedure call */
static void isa_call(Thread *t)
{
	NOT_IMPLEMENTED("call");
}

/* decrements $top */
static void isa_dec(Thread *t)
{
	NOT_IMPLEMENTED("dec");
}

/* deletes object referenced by $top and pops $top */
static void isa_delete(Thread *t)
{
	NOT_IMPLEMENTED("delete");
}

/* stops the program */
static void isa_exit(Thread *t)
{
	t->status = THS_EXIT;
}

/* makes far procedure call */
static void isa_farcall(Thread *t)
{
	NOT_IMPLEMENTED("farcall");
}

/* jumps to given address */
static void isa_farjump(Thread *t)
{
	NOT_IMPLEMENTED("farjump");
}

/* pushes constant on stack */
static void isa_getc(Thread *t)
{
	NOT_IMPLEMENTED("getc");
}

/* pushes module variable on stack */
static void isa_getd(Thread *t)
{
	NOT_IMPLEMENTED("getd");
}

/* pushes local variable on stack */
static void isa_gets(Thread *t)
{
	NOT_IMPLEMENTED("gets");
}

/* increments $top */
static void isa_inc(Thread *t)
{
	NOT_IMPLEMENTED("inc");
}

/* jumps to ($ip + $op) */
static void isa_jump(Thread *t)
{
	NOT_IMPLEMENTED("jump");
}

/* pops two stack words and jumps to ($ip + $op) if they are equal */
static void isa_jumpeq(Thread *t)
{
	NOT_IMPLEMENTED("jumpeq");
}

/* pops $top and jumps to ($ip + $op) if $top is less than zero */
static void isa_jumpl(Thread *t)
{
	NOT_IMPLEMENTED("jumpl");
}

/* pops $top and jumps to ($ip + $op) if $top is less or equal to zero */
static void isa_jumple(Thread *t)
{
	NOT_IMPLEMENTED("jumple");
}

/* pops $top and jumps to ($ip + $op) if $top is zero */
static void isa_jumpz(Thread *t)
{
	NOT_IMPLEMENTED("jumpz");
}

/* creates new module object and pushes reference to it on stack */
static void isa_new(Thread *t)
{
	NOT_IMPLEMENTED("new");
}

/* nop does nothing */
static void isa_nop(Thread *t)
{
	NOT_IMPLEMENTED("nop");
}

/* pops stack word and outputs it in given file */
static void isa_out(Thread *t)
{
	NOT_IMPLEMENTED("out");
}

/* pops stack word into module variable */
static void isa_putd(Thread *t)
{
	NOT_IMPLEMENTED("putd");
}

/* pops stack word into local variable */
static void isa_puts(Thread *t)
{
	NOT_IMPLEMENTED("puts");
}

/* returns execution from current procedure with return value */
static void isa_return(Thread *t)
{
	NOT_IMPLEMENTED("return");
}

/* returns execution from current procedure without return value */
static void isa_retp(Thread *t)
{
	NOT_IMPLEMENTED("retp");
}

/* pops two stack words and pushes their difference */
static void isa_sub(Thread *t)
{
	NOT_IMPLEMENTED("sub");
}

