#include "thread.h"
#include "module.h"
#include "isa.h"
#include "opcode.h"
#include "debug.h"

Thread::Thread()
{
	stack.allocate(STACK_SIZE);
}

Thread::~Thread()
{
}

Debug& operator<<(Debug& stream, const Thread& th)
{
	return stream << th.text() << " " << th.stack;
}

void Thread::set_module(Module& module)
{
	current_module = &module;
	current_text = &module.text;
}

void Thread::run(Module& module)
{
	files[0].bind(2);
	module.text.jump(module.ctor());
	set_module(module);
	while(!module.text.stopped()) {
		const Instruction& instr = module.text.fetch();
		//std::cout << (int)instr.opcode() << ":" << (int)instr.operand() << "\n";
		InstructionPtr ptr = instr_by_code(instr.opcode());
		ptr(*this);
	}
}

