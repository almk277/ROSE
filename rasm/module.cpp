#include "module.hpp"
#include <limits>
#include <type_traits>
#include <algorithm>

using std::string;
using std::ostream;
using error = std::runtime_error;
using buf_it = std::ostreambuf_iterator<char>;

module::module() noexcept:
	header {
		{ RMD_H_IDENT1, RMD_H_IDENT2, RMD_H_IDENT3, RMD_H_IDENT4 },
		{ 1, 0 }
	}
{}


void vartbl::add(const string &s)
{
	auto slot = map.size();
	if (slot > std::numeric_limits<addr>::max())
		throw error(string("table ") + name + " overflow");
	addr a = static_cast<addr>(slot);
	addrmap::add(s, a);
}


// zero address is not used
symbols::symbols() noexcept:
	data(1, '\0')
{}

auto symbols::add(const string &s) -> addr
{
	auto it = tbl.find(s);
	if (it != tbl.end())
		return it->second;

	addr a = static_cast<addr>(data.size());
	tbl[s] = a;
	data.insert(data.end(), s.begin(), s.end());
	data.push_back('\0');

	if (data.size() > std::numeric_limits<addr>::max())
		throw error(".sym section overflow");


	return a;
}

ostream &operator<<(ostream &s, const symbols &sym)
{
	return s.write(&sym.data.front(), sym.data.size());
}


void bytearray::put(const std::vector<char> &v)
{
	data.insert(data.end(), v.begin(), v.end());
}


ostream &operator<<(ostream &s, const bytearray &a)
{
	return s.write(&a.data.front(), a.data.size());
}


void strings::start(const string &name)
{
	cur_len_idx = data.size();
	tbl[name] = cur_len_idx;
	put(addr()); // placeholder
}

void strings::finish()
{
	addr size = data.size() - cur_len_idx - sizeof(addr);
	patch(cur_len_idx, size);
}

auto strings::operator[](const string &s) const -> addr
{
	auto it = tbl.find(s);
	if (it == tbl.end())
		throw error(s + ": no such defined string");
	return it->second;
}

void code::add_label(const string &s)
{
	auto res = lbl.insert({s, address()});
	if (not res.second)
		throw error(std::string("table ") + "label "
				+ " already has an element " + s);
}

auto code::offset(addr from, addr to,
		const string &label, int line) -> ofs
{
	ofs d = to - from;
	if ((from + d) != to)
		throw error("too long reference to " + label
				+ " at " + std::to_string(line));
	return d;
}

void code::add_ref(const std::string &label, int line)
{
	auto it = lbl.find(label);
	if (it != lbl.end()) {
		put(offset(cur_addr, it->second, label, line));
	} else {
		unres.emplace_back(unresolved{label, cur_addr,
				address(), line});
		put(ofs()); // placeholder
	}
}

void code::add_var(const string &name)
{
	var.add(name);
	++cur_proc->varc;
}

void code::add_arg(const string &name, const string &type)
{
	var.add(name);
	++cur_proc->argc;
	cur_type += type;
}

void code::finish()
{

	for (const auto &ur: unres) {
		auto it = lbl.find(ur.label);
		if (it == lbl.end())
			throw error("unresolved: " + ur.label
					+ " at " + std::to_string(ur.lineno));
		auto o = offset(ur.offset_base, it->second,
				ur.label, ur.lineno);
		patch(ur.patch_addr, o);
	}

	cur_proc->size = address() - cur_proc->addr;
	cur_proc->type = sym.add(cur_type);
	cur_proc->varc = var.count();

	var.clear();
	lbl.clear();
	unres.clear();
	cur_type.clear();
}

void code::start(RMDProcedure *p)
{
	cur_proc = p;
	cur_proc->addr = address();
}

class serializer {
public:
	explicit constexpr serializer(buf_it &it) noexcept: it(it) {}
	template<typename T> serializer &operator<<(const T &arg)
	{
		serial(it, arg);
		return *this;
	}
	serializer &operator<<(const RMDVersion &v)
	{
		return *this << v.maj << v.min;
	}
	template<std::size_t N>
		serializer &operator<<(const unsigned char (&a)[N])
	{
		std::copy_n(std::begin(a), N, it);
		return *this;
	}
private:
	buf_it &it;
};

void write(buf_it &it, const RMDExport &e)
{
	serializer(it) << e.name << e.idx;
}

void write(buf_it &it, const RMDProcedure &p)
{
	serializer(it) <<  p.addr << p.size << p.type << p.argc << p.varc;
}

void write(buf_it &it, const RMDModule &m)
{
	serializer(it) << m.name << m.version;
}

void write(buf_it &it, const RMDImport &i)
{
	serializer(it) << i.name << i.type << i.slot << i.module;
}

ostream &operator<<(ostream &s, const RMDHeader &h)
{
	buf_it it(s);
	serializer(it) << h.ident << h.rmd_version << h.name << h.version
		<< h.sizes.exp << h.sizes.ptbl << h.sizes.mtbl
		<< h.sizes.imp << h.sizes.text << h.sizes.sym << h.sizes.str
		<< h.datac << h.flags << h.size << h.pad;
	return s;
}

void module::finish()
{
	if (symbols::is_empty(header.name))
		throw error("module name was not specified");
	header.datac = data.count();
	header.sizes.exp = exp.count();
	header.sizes.ptbl = ptbl.count();
	header.sizes.mtbl = mtbl.count();
	header.sizes.imp = imp.count();
	header.sizes.text = text.count();
	header.sizes.sym = sym.count();
	header.sizes.str = str.count();
	header.size = exp.size() + ptbl.size() + mtbl.size() + imp.size()
		+ text.size() + sym.size() + str.size();
}

ostream &operator<<(ostream &s, const module &m)
{
	s << m.header << m.exp << m.ptbl << m.mtbl << m.imp
		<< m.text << m.sym << m.str;
	return s;
}
