#include "thread.h"
#include "module.h"
#include "symbol.h"

extern const R_Byte startup_code[];

int thread_init(Thread *t, struct Module *m, const Symbol *proc)
{
	RA_Export hint = 0;
	const RMDExport *exp;
	const RMDProcedure *ptbl_ent;
	if(!module_find_proc(m, proc, &hint)) {
		symbol_print(m->name);
		putc(':', stdout);
		symbol_print(proc);
		puts(": procedure not found");
		return -1;
	}

	/* TODO checks? */
	exp = &m->seg.exp.start[hint];
	ptbl_ent = &m->seg.ptbl.start[exp->idx];
	t->pc.byte = m->seg.text.start + ptbl_ent->addr;
	t->module = m;

	t->vars = t->vstack;
	t->vars_end = t->vars + sizeof t->vstack / sizeof t->vstack[0] - 1;

	/* Create first stack frame manually as if startup code
	   calls function @proc. There is only 'exit' instruction
	   in startup code, so we do not need other fields */
	t->pstack[0].retaddr = startup_code;
	t->pstack[0].proc = ptbl_ent;
	t->procs = t->pstack;
	t->procs_end = t->procs + sizeof t->pstack / sizeof t->pstack[0] - 1;
	return 0;
}

