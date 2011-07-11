#ifndef ROSE_DATA_H
#define ROSE_DATA_H

#include "segment.h"
#include <stdint.h>

class Data: public Segment {
	public:
		Data();
		~Data();

	private:
		Data(const Data&);

		int32_t *start() const { return reinterpret_cast<int32_t *>(_start); }
		int32_t operator[](int idx) const { return start()[idx]; }
		int32_t& operator[](int idx) { return start()[idx]; }
};

#endif

