/* Virtual processor */

#include "thread.h"
#include "instr.h"
#include <stdio.h>
#include <stdlib.h>

#define fetch_byte() (*t->pc++)

#define BYTE(x) R_Byte x = fetch_byte()
#define WORD(x) R_Word x = *((R_Word*)t->pc)++
#define INT(x)  R_Word *x = &t->vars[fetch_byte()]
#define FLT(x)  float *x = (float*)&t->vars[fetch_byte()]

void thread_run(Thread *t)
{
	while(1) {
		BYTE(code);
		switch(code) {
		case I_nop:
			break;
		case I_exit:
			goto out;
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
		default:
			fprintf(stderr, "Fatal error: no such instruction: %d\n", code);
			exit(1);
		}
	}
out:
}

