#include "parser.hpp"
#include "scanner.hpp"
#include "instr.hpp"
#include "module.hpp"
#include <unordered_map>
#include <functional>

using std::string;
using error = std::runtime_error;

source_error::source_error(int line, const string &msg):
	std::runtime_error(msg),
	lineno(line)
{}

string mangle(const string &type)
{
	static const std::unordered_map<string, string> types = {
		{ "int", "i" },
		{ "float", "f" },
		{ "array", "a" },
		{ "bytearray", "b" },
		{ "file", "F" },
		{ "object", "o" },
	};

	auto it = types.find(type);
	if (it == types.end())
		throw error("wrong type " + type);
	return it->second;
}

class parser_state {
public:
	enum state {
		NONE,
		SUB,
		STR,
	};

	void set(state s, std::function<void(void)> dtor = []{})
	{
		dtor_fn();
		dtor_fn = dtor;
		st = s;
	}

	void ensure(state s) const
	{
		if (st != s)
			throw error("section " + state_str(s)
					+ " expected, but section "
					+ state_str(st) + " is opened");
	}
private:
	state st = NONE;
	std::function<void(void)> dtor_fn = []{};

	static string state_str(state s)
	{
		switch (s) {
		case NONE: return "none";
		case SUB: return ".sub";
		case STR: return ".str";
		default: throw std::logic_error("wrong parser state");
		}
		return "";
	}
};

parser::parser(scanner &scn, module &mod) noexcept:
	s(scn), m(mod)
{}

void parser::parse()
{
	parser_state state;
	string name;

	bool more_tok = true;
	while (more_tok) {
		switch (s.next()) {
		case tok::STRING:
		{
			state.ensure(parser_state::SUB);
			auto it = instr.find(s.str());
			if (it == instr.end())
				throw error(s.str() + ": no such instruction");
			instruction i = it->second;
			m.text.set_next_addr(i.len);
			m.text.put(i.opcode);
			parse_operands(i);
			s >> eol;
			break;
		}
		case tok::LABEL:
			s >> name >> eol;
			m.text.add_label(name);
			break;
		case tok::BYTE:
			state.ensure(parser_state::STR);
			m.str.put(s.num_byte());
			break;
		case tok::INT:
			state.ensure(parser_state::STR);
			m.str.put(s.num_int());
			break;
		case tok::FLOAT:
			state.ensure(parser_state::STR);
			m.str.put(s.num_flt());
			break;
		case tok::ARRAY:
			state.ensure(parser_state::STR);
			m.str.put(s.array());
			break;
		case tok::NEWLINE:
			break;
		case tok::END:
			more_tok = false;
			break;
		case tok::ARG:
		{
			state.ensure(parser_state::SUB);
			string type;
			s >> type >> name >> eol;
			m.text.add_arg(name, mangle(type));
			break;
		}
		case tok::DATA:
		{
			string type;
			s >> type >> name >> eol;
			m.data.add(name);
			break;
		}
		case tok::EXPORT:
		{
			s >> name >> eol;
			auto slot = m.ptbl[name];
			auto &exp = m.exp.add(name);
			exp.name = m.sym.add(name);
			exp.idx = slot;
			break;
		}
		case tok::EXTERN:
		{
			s >> name >> eol;
			auto ss = s.string_pair();
			auto &imp = m.imp.add(name);
			imp.name = m.sym.add(ss.first);
			imp.module = m.mtbl[ss.second];
			imp.type = m.sym.add(read_types());
			break;
		}
		case tok::IMPORT:
		{
			unsigned char maj, min;
			s >> name >> maj >> min >> eol;
			auto &mod = m.mtbl.add(name);
			mod.name = m.sym.add(name);
			mod.version = { maj, min };
			break;
		}
		case tok::MODULE:
		{
			unsigned char maj, min;
			s >> name >> maj >> min >> eol;
			m.header.name = m.sym.add(name);
			m.header.version = { maj, min };
			break;
		}
		case tok::STR:
			state.set(parser_state::STR, [this]{ m.str.finish(); });
			s >> name >> eol;
			m.str.start(name);
			break;
		case tok::SUB:
			state.set(parser_state::SUB, [this]{ m.text.finish(); });
			s >> name >> eol;
			m.text.start(&m.ptbl.add(name));
			break;
		case tok::VAR:
			state.ensure(parser_state::SUB);
			s >> name >> eol;
			m.text.add_var(name);
			break;
		default: throw std::logic_error("bad token type");
		}
	} //while (more_tok)

	state.set(parser_state::NONE);
}

void parser::parse_operands(const instruction &instr)
{
	string name;
	for (auto op = instr.operands; *op; ++op) {
		switch (*op) {
		case '-': break;
		case 'c':
			m.text.put(s.read_byte());
			break;
		case 'w':
			m.text.put(s.read_int());
			break;
		case 's':
		case 'v':
		case 'i':
		case 'f':
		case 'a':
		case 'b':
		case 'F':
		case 'o':
			s >> name;
			m.text.put(m.text.find_var(name));
			break;
		case 'D':
			s >> name;
			m.text.put(m.data[name]);
			break;
		case 'P':
			s >> name;
			m.text.put(m.ptbl[name]);
			break;
		case 'A':
			s >> name;
			m.text.put(m.str[name]);
			break;
		case 'M':
			s >> name;
			m.text.put(m.mtbl[name]);
			break;
		case 'I':
			s >> name;
			m.text.put(m.imp[name]);
			break;
		case 'r':
			s >> name;
			m.text.add_ref(name, s.line());
			break;
		default: throw std::logic_error("wrong operand type");
		} // switch (*op)
	} // for
}

string parser::read_types()
{
	string type;
	bool more_types = true;
	while (more_types) {
		switch (s.next()) {
		case tok::STRING:
			type += mangle(s.str());
			break;
		case tok::NEWLINE: 
			// fallthrough
		case tok::END:
			more_types = false;
			break;
		default: throw error(s.str() + ": type expected here");
		}
	}
	return type;
}

void parser::run()
{
	try {
		parse();
		m.finish();
	} catch (std::runtime_error &re) {
		throw source_error(s.line(), re.what());
	}
}
