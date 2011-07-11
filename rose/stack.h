#ifndef ROSE_STACK_H
#define ROSE_STACK_H

#include "segment.h"
#include <stdint.h>

class Debug;

// This segment contains procedure parameters,
// local variables and some internal data
class Stack: public Segment {
	public:
		Stack();
		~Stack();
		void push(int32_t word);
		int32_t pop();
		int32_t top() const;
		void reset();
		//int32_t current_module() const { return mod; }
		int32_t operator[](int idx) const { return start()[lv + idx]; }
		int32_t& operator[](int idx) { return start()[lv + idx]; }
		int get_sp() const { return sp; }
		void set_sp(int val) { sp = val; }
		int get_lv() const { return lv; }
		void set_lv(int val) { lv = val; }
		int entsize() const { return 4; }
	private:
		Stack(const Stack&);
		int sp;
		int lv;
		int32_t mod;
		int32_t *start() const { return reinterpret_cast<int32_t *>(_start); }
		friend Debug& operator<<(Debug& stream, const Stack& rhs);
};

Debug& operator<<(Debug& stream, const Stack& rhs);

#endif

