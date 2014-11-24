/* Virtual processor */

#include "thread.h"
#include "instr.h"
#include "module.h"
#include <stdio.h>
#include <stdlib.h>

#define fetch_byte(th)   (*th->pc.byte++)
#define fetch_offset(th) (*th->pc.ofs++)
#define fetch_word(th)   (*th->pc.word++)
#define fetch_var(th)    (&th->vars[fetch_byte(th)])

#define SEG     (t->module->seg)
#define BYTE(x) R_Byte x = fetch_byte(t)
#define WORD(x) R_Word x = fetch_word(t)
#define INT(x)  R_Word *x = fetch_var(t)
#define FLT(x)  float *x = ((float*)fetch_var(t))
#define OFS(x)  RA_TextOffset x = fetch_offset(t)
#define PROC(x) RA_Proc x = fetch_byte(t)

#define jump(ofs) (t->pc.byte += ofs)

static void save_procedure(Thread *t)
{
	ActivRecord *r = ++t->procs;
	r->module = t->module;
	r->varbase = t->vars;
	r->retaddr = t->pc.byte;
}

/*
static void alloc_frame(Thread *t, const RMDProcedure *p)
{
	const RMDProcedure *cp = t->proc;
	int cur_len = cp->argc + cp->varc;
	R_Byte = 
}
*/

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
			/*PROC(p_idx);*/
			/*const RMDProcedure *p = &SEG.ptbl.start[p_idx];*/
			/*const RA_Text addr = p->addr;*/
			save_procedure(t);
			break;
		}
		case I_return:
		{
			break;
		}
		default:
			fprintf(stderr, "Fatal error: no such instruction: %d\n", code);
			exit(1);
		}
	}
}

