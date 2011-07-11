#include "segment.h"

void Segment::allocate(int size) throw(std::bad_alloc)
{
	length = size;
	_start = new char[size];
}

void Segment::free()
{
	if(_start) {
		delete[] _start;
		_start = 0;
	}
}

std::ifstream& operator>>(std::ifstream& stream, Segment& rhs) throw(std::string)
{
	stream.read(rhs._start, rhs.length);
	if(!stream)
		throw rhs.name;
	return stream;
}

