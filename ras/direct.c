#include "direct.h"
#include "tables.h"
#include "text.h"
#include "common.h"
#include "proc.h"

const struct Directive *ras_direct_find(const char *str, unsigned int len);

void read_direct(const char *direct, char *args)
{
	const struct Directive *d = ras_direct_find(direct, strlen(direct));
	if(d)
		d->func(args);
	else
		error("unknown directive");
}

static void set_current_section(int sect, const char *name)
{
	if(cur_sect == SECT_PROC)
		proc_finish();
	cur_sect = sect;
	debug_line("%s section", name);
}

void dir_const(char *args)
{
	if(*args)
		unexpect_sym("after section name");
	set_current_section(SECT_CONST, "const");
}

void dir_data(char *args)
{
	if(*args)
		unexpect_sym("after section name");
	set_current_section(SECT_DATA, "data");
}

static void check_word_is_last(char *word, const char *msg)
{
	go_to_space(&word);
	pass_spaces(&word);
	if(*word)
		unexpect_sym(msg);
}

void dir_sub(char *args)
{
	if(!*args)
		error("procedure name missing");
	cur_proc = proc_add(args);
	check_word_is_last(args, "after procedure name");
	set_current_section(SECT_PROC, "procedure");
}

void dir_arg(char *args)
{
	if(!*args)
		error("argument name missing");
	var_add(args);
	check_word_is_last(args, "after argument");
	debug_line("argument '%s'", args);
}

void dir_var(char *args)
{
	if(!*args)
		error("variable name missing");
	var_add(args);
	check_word_is_last(args, "after variable");
	debug_line("variable '%s'", args);
}

static void parse_module_name(const char *args, char *name, int *maj, int *min)
{
	if(sscanf(args, "%[^-]-%d.%d", name, maj, min) != 3)
		error("module must be specified as 'name-maj.min'");
	check_name(name);
}

void dir_import(char *args)
{
	int maj, min;
	char name[128];
	parse_module_name(args, name, &maj, &min);
	module_add(name, maj, min);
	check_word_is_last(args, "after import module name");
	debug_line("import module '%s' version %d.%d", name, maj, min);
}

void dir_module(char *args)
{
	int maj, min;
	char name[128];
	parse_module_name(args, name, &maj, &min);
	header.name = sym_add(name);
	header.version[0] = maj;
	header.version[1] = min;
	check_word_is_last(args, "after module name");
	debug_line("module is '%s'", args);
}

void dir_export(char *args)
{
	if(!cur_proc)
		error("no current procedure");
	exp_add(cur_proc);
	if(*args)
		unexpect_sym("after .export");
	debug_line("exported: '%s'", cur_proc->name);
}

void dir_extern(char *args)
{
	imp_add(args);
	check_word_is_last(args, "after procedure name");
	debug_line("extern: '%s'", args);
}

