#include "module.h"
#include "debug.h"
#include <string.h>
#include <stdint.h>

Module::Module(const std::string& fname) throw(ModuleException):
	name(fname), file(fname.c_str(), ios::in | ios::binary)
{
	if(!file.is_open())
		throw ModuleException(RMOD_OPEN);
	read_header();
	read_segment(text, header.text);
	read_segment(sym_table, header.symbols);
	read_segment(proc_table, header.proc);
	read_segment(mod_table, header.modules);
	read_segment(var_table, header.var);
	create_imp_tbl();
}

Module::~Module()
{
}

bool Module::check_signature(const unsigned char *given)
{
	static char signature[] = H_IDENT_INIT;
	return memcmp(signature, given, sizeof signature) == 0;
}

void Module::read_header()
{
	file.read(reinterpret_cast<char *>(&header), sizeof header);
	if(!file)
		throw ModuleException(RMOD_SHORT);
	if(!check_signature(header.ident))
		throw ModuleException(RMOD_SIGN);
}

void Module::read_segment(Segment& segment, const Section& section)
{
	if(section.size != 0) {
		segment.allocate(section.size);
		file.seekg(section.offset);
		file >> segment;
		segment.set_size(section.size / segment.entsize());
	}
}

void Module::create_imp_tbl()
{
	for(int i = 0; i != mod_table.size(); ++i)
		std::cout << getname(mod_table[i].name) << std::endl;
}

