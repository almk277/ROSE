#ifndef ROSE_ISA_H
#define ROSE_ISA_H

/*******************************************
 *
 * This describes entire instruction set
 *
 ******************************************/

class Thread;

typedef void (*InstructionPtr)(Thread&);

#define DEF_INSTR(instr)	void isa_ ## instr (Thread&)

/* dummy instruction */
DEF_INSTR(nop);
DEF_INSTR(push);
DEF_INSTR(pop);
DEF_INSTR(dup);
/* pops two stack words and pushes their bitwise "and" */
DEF_INSTR(and);
/* pops two stack words and pushes their bitwise "or" */
DEF_INSTR(or);
/* pops two stack words and pushes their arithmetic "+" */
DEF_INSTR(add);
/* pops two stack words and pushes their arithmetic "-" */
DEF_INSTR(sub);
/* pops two stack words and pushes their arithmetic "*" */
DEF_INSTR(mul);
/* pops two stack words and pushes their arithmetic "/" */
DEF_INSTR(div);
DEF_INSTR(swap);
DEF_INSTR(in);
DEF_INSTR(out);
DEF_INSTR(jmpf);
DEF_INSTR(jmpb);
DEF_INSTR(jz);
DEF_INSTR(jl);
DEF_INSTR(load);
DEF_INSTR(store);
DEF_INSTR(call);
DEF_INSTR(ret);
DEF_INSTR(alloc);
DEF_INSTR(free);
/* creates single module instance and pushes it's address on stack */
DEF_INSTR(pushmod);
/* stops program execution */
DEF_INSTR(exit);

/* this is called on wrong opcode */
void isa__error(Thread&);

#endif

