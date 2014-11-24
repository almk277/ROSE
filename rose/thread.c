#include "thread.h"
#include "module.h"
/*#include "compiler.h"*/

void thread_init(Thread *t)
{
	t->vars = t->vstack;
	t->procs = t->pstack;
}

inline void thread_set_module(Thread *t, Module *m)
{
	t->module = m;
	t->text = &m->seg.text;
}

int thread_jump_to(Thread *t, struct Module *m, const struct Symbol *proc)
{
	RA_Export hint = 0;
	if(module_find_proc(m, proc, &hint)) {
		const RMDExport *exp = &m->seg.exp.start[hint];
		const RMDProcedure *ptbl_ent = &m->seg.ptbl.start[exp->idx];
		t->pc.byte = m->seg.text.start + ptbl_ent->addr;
		thread_set_module(t, m);
		return 1;
	}
	return 0;
}

