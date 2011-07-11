#include "text.h"
#include "debug.h"

Text::Text():
	Segment(".text"), stp(0)
{
}

Text::~Text()
{
}

const Instruction& Text::fetch()
{
	return instr = (*this)[pc++];
}

Debug& operator<<(Debug& stream, const Text& rhs)
{
	return stream << "pc = " << rhs.pc << " (pc) = " << rhs.instr;
}

void Text::reset()
{
	stp = 0;
}

