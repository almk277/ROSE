#ifndef ROSE_MODULETABLE_H
#define ROSE_MODULETABLE_H

#include "segment.h"
#include "rof.h"

class ModuleTable: public Segment {
	public:
		ModuleTable(): Segment(".mod") {}
		ModuleEntry *start() const { return reinterpret_cast<ModuleEntry *>(_start); }
		ModuleEntry& operator[](int idx) { return start()[idx]; }
		int entsize() const { return sizeof(ModuleEntry); }
};

#endif

