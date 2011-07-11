#ifndef ROSE_VARTABLE_H
#define ROSE_VARTABLE_H

#include "segment.h"

class VarTable: public Segment {
	public:
		VarTable(): Segment(".var") {}
		int entsize() const { return sizeof(Symbol); }
};

#endif

