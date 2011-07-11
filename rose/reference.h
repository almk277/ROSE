#ifndef ROSE_REFERENCE_H
#define ROSE_REFERENCE_H

#include <stdint.h>

class Reference {
	public:
		Reference(char *val): ref(val) {}
		Reference(int32_t val): ref(reinterpret_cast<char *>(val)) {}

		operator char*() { return ref; }
		operator int32_t() { return reinterpret_cast<int32_t>(ref); }
	private:
		char *ref;
};

#endif

