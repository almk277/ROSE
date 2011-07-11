#include "isa.h"
#include "thread.h"
#include "debug.h"
#include "text.h"
#include "module.h"
#include "instr.h"
#include <new>
#include "reference.h"

#define ST	(th.stack)
#define TX	(th.text())
#define OP	(TX.CI().operand())
#define OPS	(TX.CI().operand_s())

void isa_nop(Thread& th)
{
	// Nothing to do
}

void isa_push(Thread& th)
{
	ST.push(OP);
}

void isa_pop(Thread& th)
{
	ST.pop();
}

void isa_dup(Thread& th)
{
	ST.push(ST.top());
}

void isa_and(Thread& th)
{
	int32_t left = ST.pop();
	int32_t right = ST.pop();
	ST.push(left & right);
}

void isa_or(Thread& th)
{
	int32_t left = ST.pop();
	int32_t right = ST.pop();
	ST.push(left | right);
}

void isa_add(Thread& th)
{
	int32_t left = ST.pop();
	int32_t right = ST.pop();
	ST.push(left + right);
}

void isa_sub(Thread& th)
{
	int32_t left = ST.pop();
	int32_t right = ST.pop();
	ST.push(left - right);
}

void isa_mul(Thread& th)
{
	int32_t left = ST.pop();
	int32_t right = ST.pop();
	ST.push(left * right);
}

void isa_div(Thread& th)
{
	int32_t left = ST.pop();
	int32_t right = ST.pop();
	ST.push(left / right);
}

void isa_swap(Thread& th)
{
	int32_t high = ST.pop();
	int32_t low = ST.pop();
	ST.push(high);
	ST.push(low);
}

void isa_in(Thread& th)
{
	File& file = th.files[OP];
	int32_t input;
	file >> input;
	ST.push(input);
}

void isa_out(Thread& th)
{
	File& file = th.files[OP];
	file << ST.pop();
}

static inline void jump_f(Thread& th)
{
	TX.jump_fwd(OP);
}

static inline void jump_b(Thread& th)
{
	TX.jump_back(OP);
}

void isa_jmpf(Thread& th)
{
	jump_f(th);
}

void isa_jmpb(Thread& th)
{
	jump_b(th);
}

void isa_jz(Thread& th)
{
	int32_t top = ST.pop();
	if(top == 0)
		jump_b(th);
}

void isa_jl(Thread& th)
{
	int32_t top = ST.pop();
	if(top < 0)
		jump_b(th);
}

void isa_load(Thread& th)
{
	int32_t value = ST[OPS];
	ST.push(value);
}

void isa_store(Thread& th)
{
	int32_t value = ST.pop();
	ST[OPS] = value;
}

void isa_call(Thread& th)
{
	// Stack looks like: arg1 | arg2 | ref | oldpc | oldlv | argcount
	//                                 ^lv                   ^sp
	// Store procedure information:
	// Store current module reference
	ST.push(reinterpret_cast<int32_t>(&th.current()));
	// Store program counter
	ST.push(TX.get_pc());
	// Store stack frame base
	ST.push(ST.get_lv());

	// Jump to new location
	int proc_pc = th.current().get_proc(OP);
	TX.jump(proc_pc);
	// Get procedure information
	const Instruction& instr = TX.fetch();
	uint8_t arg_count = instr.opcode();
	uint8_t var_count = instr.operand();

	// Store argument count
	ST.push(arg_count);

	// Set new stack state:
	// Set new stack frame base
	ST.set_lv(ST.get_sp() - 3);
	// Allocate local variables room
	ST.set_sp(ST.get_sp() + var_count);
}

void isa_ret(Thread& th)
{
	// Return back
	TX.jump(ST[1]);
	// Saved sp
	ST.set_sp(ST.get_lv() - ST[3]);
	// Saved LV
	ST.set_lv(ST[2]);
	// Now return value is on the stack top
}

void isa_alloc(Thread& th)
{
	int32_t size = ST.pop();
	Reference ref = new (nothrow) char[size];
	ST[OPS] = ref;
}

void isa_free(Thread& th)
{
	Reference ref = ST[OPS];
	//FIXME
	delete[] ref;
}

void isa_pushmod(Thread& th)
{
}

void isa_exit(Thread& th)
{
	TX.stop();
}

void isa__error(Thread& th)
{
	TX.stop();
	debug << "wrong opcode " << th << "\n";
}

