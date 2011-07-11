#include "instr.h"
#include "debug.h"

Debug& operator<<(Debug& stream, const Instruction& rhs)
{
	return stream << (int)rhs.opcode() << ":" << (int)rhs.operand();
}

