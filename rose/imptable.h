#ifndef ROSE_IMPTABLE_H
#define ROSE_IMPTABLE_H

#include "segment.h"
#include "rof.h"

class ImportTable: public Segment {
	public:
		ImportTable(): Segment(".import") {}
		Symbol *start() const { return reinterpret_cast<Symbol *>(_start); }
		Symbol& operator[](int idx) { return start()[idx]; }
};

#endif

