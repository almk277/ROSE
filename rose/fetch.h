/* Fetching instruction data */

#ifndef FETCH_H
#define FETCH_H

#include "thread.h"
#include "serial.h"

#define MODULE  (t->procs->module)
#define SEG     (&MODULE->seg)
#define PC      (t->pc)

#define fetch(type)      (deserial(*PC.type++))
#define fetch_var(type)  ((type*)&t->vars[fetch(stack)])
#define VAR(type, x)     type *x = fetch_var(type)

/* declare and fetch operands from text segment */
#define BYTE(x) R_Byte x = fetch(byte)
#define WORD(x) R_Word x = fetch(word)
#define INT(x)  VAR(R_Word, x)
#define FLT(x)  VAR(float, x)
#define ARR(x)  VAR(Array, x)
#define BARR(x) VAR(ByteArray, x)
#define FIL(x)  VAR(File, x)
#define OBJ(x)  VAR(Object, x)
#define DATA(x) RA_Data x = fetch(data)
#define PROC(x) RA_Proc x = fetch(proc)
#define SYM(x)  RA_Symbol x = fetch(symbol)
#define AOFS(x) RA_Array x = fetch(array)
#define MOD(x)  RA_Module x = fetch(module)
#define IMP(x)  RA_Import x = fetch(import)
#define OFS(x)  RA_TextOffset x = fetch(ofs)

#endif

