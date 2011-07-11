#include "stack.h"
#include "debug.h"

Stack::Stack():
	Segment(".stack"), sp(0), lv(0), mod(0)
{
}

Stack::~Stack()
{
}

void Stack::push(int32_t word)
{
	(*this)[++sp] = word;
}

int32_t Stack::pop()
{
	return (*this)[sp--];
}

int32_t Stack::top() const
{
	return (*this)[sp];
}

void Stack::reset()
{
	sp = 0;
	lv = 0;
}

Debug& operator<<(Debug& stream, const Stack& rhs)
{
	return stream << "sp = " << rhs.sp << " lv = " << rhs.lv << " top = " << rhs.top();
}

