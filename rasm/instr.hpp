#pragma once
#include <string>
#include <unordered_map>

struct instruction {
	unsigned char opcode;
	int len;
	const char *operands;
};

extern const std::unordered_map<std::string, instruction> instr;
