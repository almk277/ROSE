/* Fetching instruction data */

#ifndef FETCH_H
#define FETCH_H

#include "thread.h"
#include "serial.h"

#define MODULE  (t->procs->module)
#define SEG     (&MODULE->seg)
#define PC      (t->pc)
#define OBJECT  (t->vars[0])

#define fetch(type)      (deserial(*PC.type++))
#define fetch_var()      (&t->vars[fetch(stack)])
#define VAL(type, x)     type *x = (type*)fetch_var()
#define REF(x)           R_Word *x = fetch_var()

/* declare and fetch operands from text segment */
#define BYTE(x) R_Byte x = fetch(byte)
#define WORD(x) R_Word x = fetch(word)
#define INT(x)  VAL(R_Word, x)
#define FLT(x)  VAL(float, x)
#define ARR(x)  VAR(RD_Array, x)
#define BARR(x) VAR(RD_ByteArray, x)
#define FIL(x)  VAR(RD_File, x)
#define OBJ(x)  VAR(RD_Object, x)
#define DATA(x) RA_Data x = fetch(data)
#define PROC(x) RA_Proc x = fetch(proc)
#define SYM(x)  RA_Symbol x = fetch(symbol)
#define AOFS(x) RA_Array x = fetch(array)
#define MOD(x)  RA_Module x = fetch(module)
#define IMP(x)  RA_Import x = fetch(import)
#define OFS(x)  RA_TextOffset x = fetch(ofs)

#endif

