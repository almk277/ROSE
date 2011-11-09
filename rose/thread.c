#include "thread.h"
#include "compiler.h"
#include "isa.c"

void thread_init(Thread *t)
{
	stack_init(&t->stack);
	stack_init(&t->proc);
}

void thread_debug(const Thread *t)
{
	stack_debug(&t->stack);
}

void thread_set_module(Thread *t, Module *m)
{
	t->module = m;
	t->text   = &m->seg.text;
}

int thread_start(Thread *t, Module *m)
{
	int exp_ent, ptbl_ent;
	uint32_t start;
	uint8_t opcode;

	thread_set_module(t, m);
	t->status = THS_RUNNING;
   	exp_ent = module_find_proc(m, "main");
	if (exp_ent == -1)
		return THS_NOENTRY;
	ptbl_ent = exp_idx_get(&m->seg.exp, exp_ent);
	start = module_proc_addr(m, ptbl_ent);
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

