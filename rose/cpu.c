/* Virtual processor */

#include "thread.h"
#include "instr.h"
#include "fetch.h"
#include "module.h"
#include "heap.h"
#include "symbol.h"
#include "array.h"
#include "object.h"
#include "file.h"
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

static void out_of_range(int idx)
{
	fprintf(stderr, "Array index (%d) out of bound\n", idx);
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
			Symbol *procname = sym_get(seg, imp->name);
			Module *m = module_get_module(MODULE, imp->module);
			RA_Export exp_idx = module_exp_get_obligatory(m, procname, imp->slot);
			const RMDExport *exp = exp_get(&m->seg, exp_idx);
			if(thread_call_intern(t, m, exp->idx))
				stack_overflow();
			break;
		}
		case I_barr_new:
		{
			REF(r); INT(size);
			RD_ByteArray *a = bytearray_new(*size);
			*r = ptr_to_ref(a);
			break;
		}
		case I_barr_load:
		{
			REF(r); WORD(addr);
			const R_Byte *source = str_get(SEG, addr);
			RD_ByteArray *a = bytearray_load(source);
			*r = ptr_to_ref(a);
			break;
		}
		case I_barr_len:
		{
			INT(len); REF(r);
			*len = bytearray_length(ref_to_ptr(*r));
			break;
		}
		case I_dbg_str:
		{
			REF(r);
			RD_ByteArray *a = ref_to_ptr(*r);
			bytearray_print(a);
			break;
		}
		case I_barr_getb:
		{
			INT(v); REF(r); INT(i);
			RD_ByteArray *a = ref_to_ptr(*r);
			if(bytearray_get_byte(a, *i, v) < 0)
				out_of_range(*i);
			break;
		}
		case I_barr_getw:
		{
			INT(v); REF(r); INT(i);
			RD_ByteArray *a = ref_to_ptr(*r);
			if(bytearray_get_word(a, *i, v) < 0)
				out_of_range(*i);
			break;
		}
		case I_barr_putb:
		{
			INT(v); REF(r); INT(i);
			RD_ByteArray *a = ref_to_ptr(*r);
			if(bytearray_put_byte(a, *i, *v) < 0)
				out_of_range(*i);
			break;
		}
		case I_barr_putw:
		{
			INT(v); REF(r); INT(i);
			RD_ByteArray *a = ref_to_ptr(*r);
			if(bytearray_put_word(a, *i, *v) < 0)
				out_of_range(*i);
			break;
		}
		case I_arr_new:
		{
			REF(r); INT(size);
			RD_Array *a = array_new(*size);
			*r = ptr_to_ref(a);
			break;
		}
		case I_arr_len:
		{
			INT(len); REF(r);
			*len = array_length(ref_to_ptr(*r));
			break;
		}
		case I_arr_get:
		{
			INT(v); REF(r); INT(i);
			RD_Array *a = ref_to_ptr(*r);
			if(array_get(a, *i, v) < 0)
				out_of_range(*i);
			break;
		}
		case I_arr_put:
		{
			INT(v); REF(r); INT(i);
			RD_Array *a = ref_to_ptr(*r);
			if(array_put(a, *i, *v) < 0)
				out_of_range(*i);
			break;
		}
		case I_move:
		{
			INT(i1); INT(i2);
			*i1 = *i2;
			break;
		}
		case I_obj_new:
		{
			REF(r); MOD(m_idx);
			Module *m = module_get_module(MODULE, m_idx);
			RD_Object *o = object_new(m);
			*r = ptr_to_ref(o);
			break;
		}
		case I_obj_load:
		{
			INT(var); DATA(d);
			RD_Object *o = ref_to_ptr(OBJECT);
			*var = object_load(o, d);
			break;
		}
		case I_obj_store:
		{
			INT(var); DATA(d);
			RD_Object *o = ref_to_ptr(OBJECT);
			object_store(o, d, *var);
			break;
		}
		case I_file_open:
		{
			REF(f); REF(name); BYTE(mode);
			RD_ByteArray *a_name = ref_to_ptr(*name);
			RD_File *file = file_open(a_name, mode);
			*f = ptr_to_ref(file);
			break;
		}
		case I_file_close:
		{
			REF(f);
			RD_File *file = ref_to_ptr(*f);
			file_close(file);
			break;
		}
		case I_file_read:
		{
			REF(f); REF(buf); INT(size);
			RD_File *file = ref_to_ptr(*f);
			RD_ByteArray *a = ref_to_ptr(*buf);
			file_read(file, a, *size);
			break;
		}
		case I_file_write:
		{
			REF(f); REF(buf); INT(size);
			RD_File *file = ref_to_ptr(*f);
			RD_ByteArray *a = ref_to_ptr(*buf);
			file_write(file, a, *size);
			break;
		}
		default:
			fprintf(stderr, "Fatal error: no such instruction: 0x%X\n", code);
			exit(1);
		} /* switch */
	} /* while */
}

