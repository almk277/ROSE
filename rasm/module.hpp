#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <iterator>
#include <cstdint>
#include <ostream>
#include <stdexcept>
#include "serial.hpp"
#include "rmd.h"

template<typename A> class addrmap {
public:
	using addr = A;

	explicit addrmap(const std::string &name) noexcept: name(name) {}
	~addrmap() = default;
	addrmap(const addrmap&) = delete;
	addrmap &operator=(const addrmap&) = delete;

	addr operator[](const std::string &s) const
	{
		auto it = map.find(s);
		if (it == map.end())
			throw std::runtime_error("table '" + name
					+ "': no entry '" + s + "'");
		return it->second;
	}
protected:
	void add(const std::string &s, addr a)
	{
		auto res = map.insert({s, a});
		if (not res.second)
			throw std::runtime_error("table " + name
					+ " already has an element " + s);
	}

	const std::string name;
	std::unordered_map<std::string, addr> map;
};

struct vartbl: public addrmap<unsigned char> {
	explicit vartbl(const std::string &name) noexcept: addrmap(name) {}
	~vartbl() = default;

	void add(const std::string& s);
	addr count() const { return map.size(); }
	void clear() { map.clear(); }
};

class symbols {
public:
	using addr = std::uint16_t;

	symbols() noexcept;
	~symbols() = default;
	symbols(const symbols&) = delete;

	addr add(const std::string &s);
	addr count() const { return data.size(); }
	addr size() const { return data.size(); }
	static bool is_empty(addr a) { return a == 0; }
	friend std::ostream &operator<<(std::ostream &s, const symbols &sym);
private:
	std::unordered_map<std::string, addr> tbl;
	std::vector<char> data;
};

class bytearray {
public:
	bytearray() = default;
	~bytearray() = default;
	bytearray(const bytearray&) = delete;
	bytearray(bytearray&&) = delete;

	template<typename T> void put(T val) noexcept
	{
		serial(std::back_inserter(data), val);
	}
	void put(const std::vector<char> &v);

	template<typename T> void patch(std::size_t idx, T val) noexcept
	{
		serial(data.begin() + idx, val);
	}
	std::size_t count() const { return data.size(); }
	std::size_t size() const { return data.size(); }
	friend std::ostream &operator<<(std::ostream &s, const bytearray &a);
protected:
	std::vector<char> data;
};

class strings: public bytearray {
public:
	using addr = std::uint32_t;

	strings() = default;
	~strings() = default;

	void start(const std::string &name);
	void finish();
	addr operator[](const std::string &s) const;

private:
	std::unordered_map<std::string, addr> tbl;
	addr cur_len_idx;
};

class code: public bytearray {
public:
	using addr = std::uint32_t;
	using ofs = std::int16_t;

	explicit code(symbols &s) noexcept: sym(s) {}
	~code() = default;

	void add_var(const std::string &name);
	void add_arg(const std::string &name, const std::string &type);
	void add_label(const std::string &s);
	void add_ref(const std::string &label, int line);
	vartbl::addr find_var(const std::string &name) const { return var[name]; }
	void start(RMDProcedure *p);
	void finish();
	static ofs offset(addr from, addr to, const std::string &label, int line);
	void set_next_addr(addr a) { cur_addr = address() + a; }

private:
	struct unresolved {
		const std::string label;
		code::addr offset_base;
		code::addr patch_addr;
		int lineno;
	};

	symbols &sym;
	vartbl var { "var" };
	std::unordered_map<std::string, code::addr> lbl;
	std::vector<unresolved> unres;
	RMDProcedure *cur_proc{};
	std::string cur_type;
	addr cur_addr{};

	addr address() const { return data.size(); }
};

template<typename T> class table: public vartbl {
public:
	explicit table(const std::string &name) noexcept: vartbl(name) {}
	~table() = default;

	T &add(const std::string &name)
	{
		vartbl::add(name);
		vec.push_back({});
		return vec.back();
	}
	friend std::ostream &operator<<(std::ostream &s, const table &t)
	{
		extern void write(std::ostreambuf_iterator<char> &it, const T &e);
		std::ostreambuf_iterator<char> it(s);
		for (auto &e: t.vec)
			write(it, e);
		return s;
	}
	std::size_t size() const { return sizeof(T) * count(); }
private:
	std::vector<T> vec;
};

struct module {
	module() noexcept;
	~module() = default;
	module(const module&) = delete;
	module &operator=(const module&) = delete;
	module(module&&) = delete;
	module &operator=(module&&) = delete;

	RMDHeader header;
	table<RMDExport> exp { ".exp" };
	table<RMDProcedure> ptbl { ".ptbl" };
	table<RMDModule> mtbl { ".mtbl" };
	table<RMDImport> imp { ".imp" };
	symbols sym;
	code text { sym };
	strings str;
	vartbl data { "data" };

	void finish();
	friend std::ostream &operator<<(std::ostream &s, const module &m);
};
