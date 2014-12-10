/* Virtual processor */

#include "thread.h"
#include "instr.h"
#include "fetch.h"
#include "module.h"
#include "symbol.h"
#include <stdio.h>
#include <stdlib.h>

#define jump(ofs) (PC.byte += ofs)

/* startup code in ROSE bytecode itself */
const R_Byte startup_code[] = {
	I_exit,
};

static void stack_overflow(void)
{
	fputs("Stack overflow!\n", stderr);
	exit(1);
}

void thread_run(Thread *t)
{
	while(1) {
		BYTE(code);
		switch(code) {
		case I_nop:
			break;
		case I_exit:
			return;
		case I_dbg_byte:
		{
			BYTE(op);
			putchar(op);
			break;
		}
		case I_dbg_flt:
		{
			FLT(op);
			printf("%f", *op);
			break;
		}
		case I_dbg_int:
		{
			INT(op);
			printf("%d", *op);
			break;
		}
		case I_int_byte:
		{
			INT(i); BYTE(n);
			*i = n;
			break;
		}
		case I_int_new:
		{
			INT(i); WORD(n);
			*i = n;
			break;
		}
		case I_int_inc:
		{
			INT(i); BYTE(c);
			*i += c;
			break;
		}
		case I_int_add:
		{
			INT(i); INT(i1); INT(i2);
			*i = *i1 + *i2;
			break;
		}
		case I_int_sub:
		{
			INT(i); INT(i1); INT(i2);
			*i = *i1 - *i2;
			break;
		}
		case I_flt_new:
		{
			FLT(x); WORD(n);
			*x = *(float*)&n;
			break;
		}
		case I_flt_add:
		{
			FLT(x); FLT(x1); FLT(x2);
			*x = *x1 + *x2;
			break;
		}
		case I_flt_sub:
		{
			FLT(x); FLT(x1); FLT(x2);
			*x = *x1 - *x2;
			break;
		}
		case I_flt_mul:
		{
			FLT(x); FLT(x1); FLT(x2);
			*x = *x1 * *x2;
			break;
		}
		case I_flt_div:
		{
			FLT(x); FLT(x1); FLT(x2);
			*x = *x1 / *x2;
			break;
		}
		case I_jmp:
		{
			OFS(ofs);
			jump(ofs);
			break;
		}
		case I_jmp_int_z:
		{
			INT(i); OFS(ofs);
			if(*i == 0)
				jump(ofs);
			break;
		}
		case I_jmp_int_eq:
		{
			INT(i1); INT(i2); OFS(ofs);
			if(*i1 == *i2)
				jump(ofs);
			break;
		}
		case I_jmp_int_l:
		{
			INT(i1); INT(i2); OFS(ofs);
			if(*i1 < *i2)
				jump(ofs);
			break;
		}
		case I_jmp_int_le:
		{
			INT(i1); INT(i2); OFS(ofs);
			if(*i1 <= *i2)
				jump(ofs);
			break;
		}
		case I_jmp_flt_l:
		{
			FLT(f1); FLT(f2); OFS(ofs);
			if(*f1 <= *f2)
				jump(ofs);
			break;
		}
		case I_jmp_ref_eq:
		{
			WORD(r1); WORD(r2); OFS(ofs);
			if(r1 == r2)
				jump(ofs);
			break;
		}
		case I_call:
		{
			PROC(p_idx);
			if(thread_call_intern(t, MODULE, p_idx))
				stack_overflow();
			break;
		}
		case I_return:
		{
			thread_return(t);
			break;
		}
		case I_invoke:
		{
			IMP(i);
			const Segments *const seg = SEG;
			const RMDImport *imp = imp_get(seg, i);
			const Symbol *procname = sym_get(seg, imp->name);
			const RMDModule *mod = mtbl_get(seg, imp->module);
			const Symbol *modname = sym_get(seg, mod->name);
			Module *m = module_get_obligatory(modname);
			RA_Export exp_idx = module_exp_get_obligatory(m, procname, imp->slot);
			const RMDExport *exp = exp_get(&m->seg, exp_idx);
			if(thread_call_intern(t, m, exp->idx))
				stack_overflow();
			break;
		}
		default:
			fprintf(stderr, "Fatal error: no such instruction: 0x%X\n", code);
			exit(1);
		} /* switch */
	} /* while */
}

