#include "thread.h"
#include "module.h"
#include "symbol.h"

extern const R_Byte startup_code[];

int thread_init(Thread *t, struct Module *m, Symbol *proc)
{
	const Segments *seg = &m->seg;
	RA_Export exp_idx = module_exp_get_obligatory(m, proc, 0);

	const RMDExport *exp = exp_get(seg, exp_idx);
	const RMDProcedure *ptbl_ent = ptbl_get(seg, exp->idx);
	t->pc.byte = text_get(seg, ptbl_ent->addr);

	t->vars = t->vstack;
	t->vars_end = t->vars + sizeof t->vstack / sizeof t->vstack[0] - 1;

	/* Create first stack frame manually as if startup code
	   calls function @proc. There is only 'exit' instruction
	   in startup code, so we do not need all fields */
	t->pstack[0].retaddr = startup_code;
	t->pstack[0].proc = ptbl_ent;
	t->pstack[0].module = m;
	t->procs = t->pstack;
	t->procs_end = t->procs + sizeof t->pstack / sizeof t->pstack[0] - 1;
	return 0;
}

static int save_procedure(Thread *t, const RMDProcedure *proc)
{
	ActivRecord *r = ++t->procs;
	if(r > t->procs_end)
		return -1;
	r->proc    = proc;
	r->varbase = t->vars;
	r->retaddr = t->pc.byte;
	return 0;
}

static void restore_procedure(Thread *t)
{
	/* do not check for underflow, because
	 * runtime code catches excess return and causes exit */
	--t->procs;
}

static int alloc_frame(Thread *t, const RMDProcedure *np)
{
	const RMDProcedure *cp = t->procs->proc; /* current procedure descriptor */
	int cur_len = cp->argc + cp->varc; /* current stack length */
	/* allocate new stack frame; arguments are in current frame */
	t->vars += cur_len - np->argc;
	if(t->vars > t->vars_end)
		return -1;
	return 0;
}

static void free_frame(Thread *t)
{
	t->vars = t->procs->varbase;
}

int thread_call_intern(Thread *t, Module *module, RA_Proc p_idx)
{
	const Segments *seg = &module->seg;
	const RMDProcedure *p = ptbl_get(seg, p_idx);
	if(alloc_frame(t, p) || save_procedure(t, p))
		return -1;
	t->pc.byte = text_get(seg, p->addr);
	t->procs->module = module;
	return 0;
}

void thread_return(Thread *t)
{
	t->pc.byte = t->procs->retaddr;
	free_frame(t);
	restore_procedure(t);
}

int thread_call_extern(Thread *t, struct Module *module, RA_Import p_idx);

