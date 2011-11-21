#include "thread.h"
#include "compiler.h"
#include "isa.c"
#include <glib.h>

void thread_init(Thread *t)
{
	stack_init(&t->stack);
	stack_init(&t->proc);
}

void thread_debug(const Thread *t)
{
	stack_debug(&t->stack);
}

inline void thread_set_module(Thread *t, Module *m)
{
	t->module = m;
	t->text   = &m->seg.text;
}

int thread_start(Thread *t, Module *m)
{
	int ent;
	uint32_t start;
	uint8_t opcode;

	thread_set_module(t, m);
	t->status = THS_RUNNING;
   	ent = module_find_proc(m, "main");
	if (ent == -1)
		return THS_NOENTRY;
	start = module_get_proc(m, ent)->addr;
	text_goto(t->text, start);

	while(likely(t->status == THS_RUNNING)) {
		opcode = text_fetch(t->text);
		t->operand = text_fetch(t->text);
		instr_run(opcode, t);

	}

	switch(t->status) {
		case THS_EXIT: break;
		case THS_INV_OPCODE:
					   printf("Invalid instruction [%x]\n", (int)opcode);
					   break;
	}
	return THS_EXIT;
}

void thread_proc_call(Thread *t, Module *m, uint8_t idx)
{
	RMDProcedure *p = module_get_proc(m, idx);
	Proc *proc = g_slice_new(Proc);
	proc->proc = p;
	proc->sp = t->stack.sp;
	proc->bp = t->stack.bp;
	proc->pc = t->text->pc;
	proc->module = m;
	proc->prev = t->current_proc;
	t->current_proc = proc;
	thread_set_module(t, m);
	text_goto(t->text, p->addr);
	/* account arguments */
	t->stack.bp = t->stack.sp - p->argc;
	/* make space for variables */
	t->stack.sp += p->varc;
}

void thread_proc_ret(Thread *t, int retval)
{
	Proc *proc = t->current_proc;
	thread_set_module(t, proc->module);
	t->stack.sp = proc->sp;
	t->stack.bp = proc->bp;
	t->text->pc = proc->pc;
	t->current_proc = proc->prev;
	/* account arguments and return value */
	t->stack.sp -= proc->proc->argc;
	t->stack.sp += retval;
	g_slice_free(Proc, proc);
}

