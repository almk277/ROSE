#include "thread.h"

#define NOT_IMPLEMENTED		;

#include "isa_desc.c"

typedef void (*Instr)(Thread *);

static void isa_error(Thread *t)
{

}

#define NO_INSTR isa_error

static const Instr instr_set[] = {
#include "isa_tbl.c"
};

void instr_run(int idx, Thread *t)
{
	instr_set[idx](t);
}

