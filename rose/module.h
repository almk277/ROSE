#ifndef ROSE_MODULE_H
#define ROSE_MODULE_H

#include "text.h"
#include "proctable.h"
#include "vartable.h"
#include "moduletable.h"
#include "symboltable.h"
#include "rof.h"
#include <string>
#include <fstream>

enum ModuleError {
	RMOD_OPEN,
	RMOD_SIGN,
	RMOD_SHORT,
};

class ModuleException {
	public:
		ModuleException(ModuleError code, const std::string& title = std::string()):
			err(code), str(title) {};
		ModuleError error() const { return err; }
		const std::string& descr() const { return str; }
	private:
		const ModuleError err;
		const std::string str;
};

// An independent program module (class)
class Module {
	public:
		Module(const std::string& fname) throw(ModuleException);
		~Module();

		Text text;
		int ctor() const { return 0; }
		int get_proc(uint8_t idx) const { return proc_table[idx].addr; }
	private:
		Module(const Module&);

		std::string name;
		std::ifstream file;
		Header header;

		SymbolTable sym_table;
		ProcTable proc_table;
		VarTable var_table;
		ModuleTable mod_table;

		void read_header();
		bool check_signature(const unsigned char *given);
		void read_segment(Segment& segment, const Section& section);
		void create_imp_tbl();
		const char *getname(int idx) { return sym_table[idx]; }
};

#endif

