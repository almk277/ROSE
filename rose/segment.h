#ifndef SEGMENT_H
#define SEGMENT_H

#include <stdint.h>

template <typename T>
class Segment {
	public:
		//T *start() const { return _start; }
		void place(const char *start, uint32_t len)
		{
			_start = reinterpret_cast<T *>(start);
			_length = len;
		}
	private:
		T *_start;
		uint32_t _length;
};

#endif

