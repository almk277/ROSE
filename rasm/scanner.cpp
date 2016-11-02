#include "scanner.hpp"
#include <cctype>
#include <limits>
#include <unordered_map>
#include <stdexcept>

using std::string;
using std::numeric_limits;
using error = std::runtime_error;

constexpr char COMMENT = ';';
constexpr char INT_SUFFIX = 'L';
constexpr char STRING_DELIM = '@';

scanner::scanner(std::istream &in) noexcept:
	in(in)
{}

string tokname(tok t)
{
	struct tokhash {
		std::size_t operator()(tok t) const
		{ return static_cast<std::size_t>(t); }
	};

	static const std::unordered_map<tok, string, tokhash> names = {
		{ tok::STRING,  "string" },
		{ tok::LABEL,   "label" },
		{ tok::BYTE,    "byte" },
		{ tok::INT,     "int" },
		{ tok::FLOAT,   "float" },
		{ tok::ARRAY,   "array" },
		{ tok::NEWLINE, "new line" },
		{ tok::END,     "end of file" },
		{ tok::ARG,     ".arg" },
		{ tok::DATA,    ".data" },
		{ tok::EXPORT,  ".export" },
		{ tok::EXTERN,  ".extern" },
		{ tok::IMPORT,  ".import" },
		{ tok::MODULE,  ".module" },
		{ tok::STR,     ".str" },
		{ tok::SUB,     ".sub" },
		{ tok::VAR,     ".var" },
	};

	return names.at(t);
}

template<typename T>
void convert(T &val, const string &s)
{
	std::size_t pos;
	long num = std::stol(s, &pos, 0);
	if (pos < s.size())
		throw error("wrong int:" + s);
	if (num < numeric_limits<T>::min()
			or num > numeric_limits<T>::max())
		throw error("int out of range: " + s);
	val = static_cast<T>(num);
}

static char unescape(char c)
{
	switch (c) {
		case '0': return '\0';
		case 'n': return '\n';
		case 't': return '\t';
	}
	return c;
}

tok scanner::next()
{
	static const std::unordered_map<string, tok> dirs = {
		{ "arg",    tok::ARG },
		{ "data",   tok::DATA },
		{ "export", tok::EXPORT },
		{ "extern", tok::EXTERN },
		{ "import", tok::IMPORT },
		{ "module", tok::MODULE },
		{ "str",    tok::STR },
		{ "sub",    tok::SUB },
		{ "var",    tok::VAR },
	};

	char c = in.peek();

	while (c == ' ' or c == '\t')
		c = in.ignore().peek();

	if (in.eof())
		return tok::END;

	if (std::isalpha(c) or c == '_') {
		in >> s;
		if (s.back() == ':') {
			s.pop_back();
			return tok::LABEL;
		}
		return tok::STRING;
	}

	if (std::isdigit(c) or c == '-') {
		in >> s;
		if (s.find('.') != string::npos) {
			std::size_t pos;
			d = std::stof(s, &pos);
			if (pos < s.size())
				throw error("wrong float: " + s);
			return tok::FLOAT;
		}
		if (s.back() == INT_SUFFIX) {
			s.pop_back();
			convert(n, s);
			return tok::INT;
		}
		convert(b, s);
		return tok::BYTE;
	}

	in.ignore();
	switch (c) {
	case '.':
	{
		if (!std::isalpha(in.peek()))
			throw error("ill-formed directive");
		in >> s;
		auto it = dirs.find(s);
		if (it == dirs.end())
			throw error("unknown directive:" + s);
		return it->second;
	}
	case '\n':
		++lineno;
		return tok::NEWLINE;
	case COMMENT:
		in.ignore(numeric_limits<std::streamsize>::max(), '\n');
		if (in.eof())
			return tok::END;
		++lineno;
		return tok::NEWLINE;
	case '"':
		a.clear();
		while (in.get(c)) {
			switch (c) {
			case '"':
				return tok::ARRAY;
			case '\n':
				throw error("unexpected end of line");
			case '\\':
				c = unescape(in.get());
				// fallthrough
			default:
				a.push_back(c);
			}
		}
		throw error("unexpected end of file");
	}

	throw error("unexpected symbol: "
			+ (std::isprint(c) ?
				string("'") + c + "'"
				: string("#") + std::to_string(int(c))));
	return tok::END; // nonreachable
}

void scanner::next(tok expected)
{
	tok t = next();
	if (expected != t)
		throw error(tokname(t) + " encountered, but "
				+ tokname(expected) + " expected");
}

std::int32_t scanner::read_int()
{
	next(tok::INT);
	return n;
}

unsigned char scanner::read_byte()
{
	next(tok::BYTE);
	return b;
}

scanner &operator>>(scanner &scn, const eol_type&)
{
	scn.next(tok::NEWLINE);
	return scn;
}

scanner &operator>>(scanner &scn, std::string &str)
{
	scn.next(tok::STRING);
	str = scn.s;
	return scn;
}

scanner &operator>>(scanner &scn, unsigned char &c)
{
	scn.next(tok::BYTE);
	c = scn.b;
	return scn;
}

scanner &operator>>(scanner &scn, std::int32_t &num)
{
	scn.next(tok::INT);
	num = scn.n;
	return scn;
}

scanner &operator>>(scanner &scn, float &num)
{
	scn.next(tok::FLOAT);
	num = scn.d;
	return scn;
}

std::pair<string, string> scanner::string_pair() const
{
	auto pos = s.find(STRING_DELIM);
	if (pos == string::npos)
		throw error(s + ": compound string expected");
	return { s.substr(0, pos), s.substr(pos) };
}
