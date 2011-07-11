#ifndef ROSE_DEBUG_H
#define ROSE_DEBUG_H

#include <iostream>

// This is used for debug output
class Debug {
};

extern Debug debug;

template <class T>
Debug& operator<<(Debug& stream, const T& rhs)
{
	std::cerr << rhs;
	return stream;
}

#endif

