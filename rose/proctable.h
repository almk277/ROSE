#ifndef ROSE_PROCTABLE_H
#define ROSE_PROCTABLE_H

#include "segment.h"
#include "rof.h"

class ProcTable: public Segment {
	public:
		ProcTable(): Segment(".proc") {}
		Symbol *start() const { return reinterpret_cast<Symbol *>(_start); }
		Symbol& operator[](int idx) const { return start()[idx]; }
		int entsize() const { return sizeof(Symbol); }
};

#endif

