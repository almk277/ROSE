#ifndef ROSE_SYMBOLTABLE_H
#define ROSE_SYMBOLTABLE_H

#include "segment.h"

class SymbolTable: public Segment {
	public:
		SymbolTable(): Segment(".sym") {}
		const char *start() const { return const_cast<const char *>(_start); }
		const char *operator[](int idx) const { return start() + idx; }
		int entsize() const { return 1; }
};

#endif

