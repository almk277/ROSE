#ifndef ROSE_OPCODE_H
#define ROSE_OPCODE_H

#include "isa.h"

// This converts operation code to function pointer
InstructionPtr instr_by_code(unsigned char code);

#endif

