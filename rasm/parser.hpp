#pragma once
#include <string>
#include <stdexcept>
class scanner;
struct module;
struct instruction;

struct source_error: std::runtime_error {
	source_error(int line, const std::string &msg);
	const int lineno;
};

class parser {
public:
	parser(scanner &scn, module &mod) noexcept;
	~parser() = default;
	parser(const parser&) = delete;
	parser &operator=(const parser&) = delete;

	void run();
private:
	scanner &s;
	module &m;

	void parse();
	void parse_operands(const instruction &instr);
	std::string read_types();
};
