#ifndef ROSE_INSTR_H
#define ROSE_INSTR_H

#include <stdint.h>
#include <endian.h>

class Debug;

struct Instruction {
	uint16_t instr;
#if __BYTE_ORDER == __LITTLE_ENDIAN
	uint8_t opcode() const { return instr; }
	uint8_t operand() const { return instr >> 8; }
	int8_t operand_s() const { return instr >> 8; }
#else
#error "I'm too lazy to think about it"
#endif
	Instruction& operator=(uint16_t in) { instr = in; return *this; }

	friend Debug& operator<<(Debug& stream, const Instruction& rhs);
};

Debug& operator<<(Debug& stream, const Instruction& rhs);

#endif

