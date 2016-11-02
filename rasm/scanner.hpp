#pragma once
#include <string>
#include <vector>
#include <istream>
#include <utility>
#include <cstdint>

enum class tok {
	STRING,
	LABEL,
	BYTE,
	INT,
	FLOAT,
	ARRAY,
	NEWLINE,
	END,

	ARG,
	DATA,
	EXPORT,
	EXTERN,
	IMPORT,
	MODULE,
	STR,
	SUB,
	VAR,
};

struct eol_type {};
constexpr eol_type eol{};

class scanner {
public:
	explicit scanner(std::istream &in) noexcept;
	~scanner() = default;
	scanner(const scanner&) = delete;
	scanner &operator=(const scanner&) = delete;
	scanner(scanner&&) = delete;
	scanner &operator=(scanner&&) = delete;

	const std::string &str() const { return s; }
	std::int32_t num_int() const { return n; }
	unsigned char num_byte() const { return b; }
	float num_flt() const { return d; }
	const std::vector<char> &array() const { return a; }
	std::pair<std::string, std::string> string_pair() const;

	tok next();
	std::int32_t read_int();
	unsigned char read_byte();

	friend scanner &operator>>(scanner &scn, const eol_type&);
	friend scanner &operator>>(scanner &scn, std::string &str);
	friend scanner &operator>>(scanner &scn, unsigned char &c);
	friend scanner &operator>>(scanner &scn, std::int32_t &num);
	friend scanner &operator>>(scanner &scn, float &num);

	int line() const { return lineno; }
private:
	std::istream &in;
	int lineno = 1;

	std::string s;
	std::int32_t n{};
	unsigned char b{};
	float d{};
	std::vector<char> a;

	void next(tok expected);
};
