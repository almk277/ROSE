#ifndef ROSE_TEXT_H
#define ROSE_TEXT_H

#include "segment.h"
#include <stdint.h>
#include "instr.h"

class Debug;

// This segment contains executable code
class Text: public Segment {
	public:
		Text();
		~Text();

		const Instruction& fetch();
		const Instruction& CI() const { return instr; }
		bool stopped() const { return stp; }
		void stop() { stp = 1; }
		void reset();
		void jump_fwd(uint8_t ofs) { pc += ofs; }
		void jump_back(uint8_t ofs) { pc -= ofs; }
		void jump(int addr) { pc = addr; }
		int get_pc() const { return pc; }
		int entsize() const { return 2; }
	private:
		Text(const Text&);
		int pc;
		int stp;
		Instruction instr;

		uint16_t *start() const { return reinterpret_cast<uint16_t *>(_start); }

		uint16_t operator[](int idx) const { return start()[idx]; }
		friend Debug& operator<<(Debug& stream, const Text& rhs);
};

Debug& operator<<(Debug& stream, const Text& rhs);

#endif

