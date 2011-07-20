#include "thread.h"
#include <stdio.h>

#define NOT_IMPLEMENTED(name)	printf("%s: not implemented\n", name)

#include "isa_desc.c"

typedef void (*Instr)(Thread *);

static void isa_error(Thread *t)
{
	t->status = THS_INV_OPCODE;
	puts("Invalid instruction");
}

#define NO_INSTR isa_error

static const Instr instr_set[] = {
#include "isa_tbl.c"
};

void instr_run(int idx, Thread *t)
{
	instr_set[idx](t);
}

