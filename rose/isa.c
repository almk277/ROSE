#include "thread.h"
#include "ref.h"
#include "array.h"
#include "conf.h"
#include <stdio.h>

#define NOT_IMPLEMENTED(name)	printf("%s: not implemented\n", name)
#define ST        (&t->stack)
#define TX        (t->text)
#define OP        (t->operand)

typedef void (*Instr)(Thread *);

static void isa_error(Thread *t)
{
	t->status = THS_INV_OPCODE;
}

#define NO_INSTR               isa_error
#define DBG_NO_INSTR           isa_error

#include "isa_desc.c"
static const Instr instr_set[] = {
#include "isa_tbl.c"
};

#ifdef ENABLE_DEBUGGER
#include "isa_dbg.c"
static const Instr instr_dbg[] = {
#include "dbg_tbl.c"
};
#endif /* ENABLE_DEBUGGER */

#define instr_run(idx, thread)  instr_set[idx](thread)

