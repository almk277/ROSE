#ifndef INSTR_H
#define INSTR_H

enum RoseInstrType {
	ROSE_ITYPE_n,    /* no operand */
	ROSE_ITYPE_c,    /* index to .const */
	ROSE_ITYPE_s,    /* index to .stack */
	ROSE_ITYPE_d,    /* index to .data */
	ROSE_ITYPE_o,    /* code offset */
	ROSE_ITYPE_i,    /* signed 8-bit number */
	ROSE_ITYPE_u,    /* unsigned 8-bit number */
	ROSE_ITYPE_a,    /* index to .addr */
	ROSE_ITYPE_p,    /* index to .ptbl */
	ROSE_ITYPE_f,    /* index to .imp */
	ROSE_ITYPE_m,    /* index to .mtbl */
};

struct RoseInstruction {
	const char *name;
	int operand;
	int opcode;
	const char *descr;
};

const struct RoseInstruction *rose_instr_find(const char *str,
		unsigned int len);

#endif

