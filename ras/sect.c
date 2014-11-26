#include "sect.h"
#include "print.h"
#include "symbol.h"
#include "symtbl.h"
#include "storage.h"
#include "rmd.h"
#include "mm.h"
#include "serial.h"
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

/* ROSE version */
#define ROSE_VERSION_MAJ   1
#define ROSE_VERSION_MIN   0

/* all names */
static SymbolTable data_tbl   = SYMBOLTABLE_INITIALIZER; /* module data */
static SymbolTable module_tbl = SYMBOLTABLE_INITIALIZER; /* imported modules */
static SymbolTable imp_tbl    = SYMBOLTABLE_INITIALIZER; /* imported methods */
static SymbolTable proc_tbl   = SYMBOLTABLE_INITIALIZER; /* procedures */
static SymbolTable label_tbl  = SYMBOLTABLE_INITIALIZER; /* labels */
static SymbolTable exp_tbl    = SYMBOLTABLE_INITIALIZER; /* export table */
static SymbolTable var_tbl    = SYMBOLTABLE_INITIALIZER; /* method variables */
static SymbolTable array_tbl  = SYMBOLTABLE_INITIALIZER; /* arrays */
static SymbolTable sym_tbl    = SYMBOLTABLE_INITIALIZER; /* symbols */

/* all sections */
static RMDModule module_sect[256];                          /* #mtbl */
static RMDImport imp_sect[256];                             /* #imp  */
static Storage sym_sect = STORAGE_INITIALIZER(sym_sect);    /* #sym  */
static Storage str_sect = STORAGE_INITIALIZER(str_sect);    /* #str  */
static RMDProcedure ptbl_sect[256];                         /* #ptbl */
static RMDExport exp_sect[256];                             /* #exp  */
static Storage text_sect = STORAGE_INITIALIZER(text_sect);  /* #text */

extern FILE *output;
extern int yylineno;
extern int verbose;
extern RA_Text next_instr_addr;
extern RA_Text sub_len;

#define REF_MIN_OFS INT16_MIN
#define REF_MAX_OFS INT16_MAX

typedef struct Reference {
	Symbol *name;                /* referenced label */
	int lineno;                  /* reference source line */
	RA_Text base;                /* reference base address */
	RA_TextOffset *value;        /* reference value address */
	SLIST_ENTRY(Reference) le;   /* list entry */
} Reference;

static SLIST_HEAD(RefList, Reference) ref_head;
static MM_DECL(Reference, 16);

static SymbolValue *current_sub = 0;
static RMDModule *current_import;

#define text_put(arg) storage_put(&text_sect, arg)

RMDHeader header = {
	{ /* ident */
		RMD_H_IDENT1,
		RMD_H_IDENT2,
		RMD_H_IDENT3,
		RMD_H_IDENT4
	},
	{ /* RMD version */
		ROSE_VERSION_MAJ,
		ROSE_VERSION_MIN
	},
	0, /* name */
	0, /* parent */
	{ 0, 0 } /* module version */
};

static SymbolValue *sym_add_uniq(SymbolTable *tbl, const Symbol *sym)
{
	SymbolValue *v = symtbl_add_unique(tbl, sym);
	if(!v)
		error_symbol(sym, "symbol is defined already");
	return v;
}

static SymbolValue *sym_index(SymbolTable *tbl, const Symbol *sym)
{
	SymbolValue *v = sym_add_uniq(tbl, sym);
	v->i = symtbl_size(tbl) - 1;
	return v;
}

static SymbolValue *sym_get_or_die(const SymbolTable *tbl, const Symbol *sym)
{
	SymbolValue *val = symtbl_find(tbl, sym);
	if(!val)
		error_symbol(sym, "symbol not found");
	return val;
}

static int sym_get_idx(const SymbolTable *tbl, const Symbol *sym)
{
	return sym_get_or_die(tbl, sym)->i;
}

static void check_current_sub()
{
	if(!current_sub)
		error("no current procedure");
}

void data_add(const Symbol *name)
{
	sym_index(&data_tbl, name);
}

static RA_Symbol sym_add(const Symbol *sym)
{
	SymbolValue *v = symtbl_find(&sym_tbl, sym);
	if(v)
		return v->i;
	v = symtbl_add_unique(&sym_tbl, sym);
	return v->i = storage_add_symbol(&sym_sect, sym);
}

void sym_write()
{
	storage_write(&sym_sect);
}

static void parse_version(const char *s, RMDVersion *ver)
{
	unsigned maj, min;
	if(sscanf(s, "%u.%u", &maj, &min) != 2)
		error("can not parse version: %s", s);
	if(maj > 255 || min > 255)
		error("%u.%u: version number is too big - must be less then 256",
				maj, min);
	ver->maj = maj;
	ver->min = min;
}

void module_add(const Symbol *name)
{
	SymbolValue *v = sym_index(&module_tbl, name);
	current_import = &module_sect[v->i];
	current_import->name = serial(sym_add(name));
	current_import->version.maj = current_import->version.min = 0;
}

void module_set_version(const char *version)
{
	parse_version(version, &current_import->version);
}

void module_write()
{
	const int size = symtbl_size(&module_tbl);
	if(fwrite(module_sect, sizeof(RMDModule), size, output) != size)
		file_write_error();
}

void imp_add(const Symbol *fullname)
{
 	SymbolValue *v = sym_index(&imp_tbl, fullname);
	RMDImport *imp = &imp_sect[v->i];
	Symbol *module, *proc;
	symbol_split_colon(fullname, &module, &proc); /* we know there is ":" */
	imp->module = sym_get_idx(&module_tbl, module);
	imp->name = serial(sym_add(proc));
	imp->slot = 0;
	symbol_delete(proc);
	symbol_delete(module);
}

void imp_write()
{
	const int size = symtbl_size(&imp_tbl);
	if(fwrite(imp_sect, sizeof(RMDImport), size, output) != size)
		file_write_error();
}

void header_set_name(const Symbol *name)
{
	if(header.name != 0)
		error("module has name already");
	header.name = serial(sym_add(name));
}

void header_set_version(const char *version)
{
	parse_version(version, &header.version);
}

void header_set_parent(const Symbol *name)
{
	if(header.parent != 0)
		error("module is inherited already");
	header.parent = serial(sym_add(name));
}

void header_write()
{
	if(fwrite(&header, sizeof(RMDHeader), 1, output) != 1)
		file_write_error();
}

enum NumberDescIdx {
	NUMDESC8,
	NUMDESC16,
	NUMDESC32,
};

static const struct NumberDesc {
	long min;
	long max;
} num_desc[] = {
	{ INT8_MIN,  INT8_MAX },
	{ INT16_MIN, INT16_MAX },
	{ INT32_MIN, INT32_MAX },
};

static char escaped_char(char sym)
{
	switch(sym) {
		case 'n':  return '\n';
		case 't':  return '\t';
		case '\\': return '\\';
		case '\'': return '\'';
		case '"': return '"';
		default: return sym;
	}
}

static long int_parse(const char *string, int code)
{
	long arg;
	if(sscanf(string, "%li", &arg) != 1)
		error("%s: not an integer", string);
	if(arg < num_desc[code].min || arg > num_desc[code].max)
		error("%ld: number out of range", arg);
	return arg;
}

static R_Word flt_parse(const char *string)
{
	float f;
	if(sscanf(string, "%f", &f) != 1)
		error("%s: not a float", string);
	return *(R_Word*)&f;
}

void str_begin(const Symbol *name)
{
	SymbolValue *v = sym_index(&array_tbl, name);
	v->u32 = array_begin(&str_sect);
}

void str_add_escaped_char(char c)
{
	array_add_byte(escaped_char(c));
}

void str_add_char(char c)
{
	array_add_byte(c);
}

void str_add_flt(const char *s)
{
	/* TODO Not implemented yet */
}

void str_add_int(const char *s)
{
	/* TODO Not implemented yet */
}

void str_end()
{
	array_end();
}

void str_write()
{
	storage_write(&str_sect);
}

static void sub_begin(SymbolValue *p);

void ptbl_add(const Symbol *name)
{
	SymbolValue *v = sym_index(&proc_tbl, name);
	RMDProcedure *p = &ptbl_sect[v->i];
	p->addr = serial((RA_Text)text_sect.len);
	p->argc = p->varc = 0;
	sub_begin(v);
}

void ptbl_write()
{
	const int size = symtbl_size(&proc_tbl);
	if(fwrite(ptbl_sect, sizeof(RMDProcedure), size, output) != size)
		file_write_error();
}

static RMDProcedure *add_to_var(const Symbol *name)
{
	check_current_sub();
	sym_index(&var_tbl, name);
	return &ptbl_sect[current_sub->i];
}

void var_add(const Symbol *name)
{
	++add_to_var(name)->varc;
}

static void var_clear()
{
	symtbl_clear(&var_tbl);
}

void arg_add(const Symbol *name)
{
	++add_to_var(name)->argc;
}

void text_enlarge(RA_Text size)
{
	storage_enlarge(&text_sect, size);
}

RA_Text text_addr()
{
	return text_sect.len;
}

void text_emit_opcode(char opcode)
{
	text_put(opcode);
}

static void ref_new(Symbol *label);

void text_emit_escaped_char(char type, char c)
{
	switch(type) {
		case 'c':
			text_put(escaped_char(c)); break;
		default:
			error("'\%c': unexpected literal", c);
	}
}

void text_emit_char(char type, char c)
{
	switch(type) {
		case 'c':
			text_put(c); break;
		default:
			error("'%c': unexpected literal", c);
	}
}

void text_emit_flt(char type, const char *c)
{
	switch(type) {
		case 'w':
			text_put(flt_parse(c)); break;
		default:
			error("%s: unexpected float constant", c);
	}
}

void text_emit_int(char type, const char *c)
{
	switch(type) {
		case 'c':
			text_put((R_Byte)int_parse(c, NUMDESC8)); break;
		case 'w':
			text_put((R_Word)int_parse(c, NUMDESC32)); break;
		default:
			error("%s: unexpected integer constant", c);
	}
}

void text_emit_symbol(char type, Symbol *symbol)
{
	switch(type) {
		case 0:
		case '-':
			error_symbol(symbol, "superfluous argument");
		case 's':
		case 'v':
		case 'i':
		case 'f':
		case 'a':
		case 'b':
		case 'F': /* they all are stack variables */
		case 'o': text_put((RA_Stack)sym_get_idx(&var_tbl, symbol)); break;

		case 'D': text_put((RA_Data)sym_get_idx(&data_tbl, symbol)); break;
		case 'P': text_put((RA_Proc)sym_get_idx(&proc_tbl, symbol)); break;
		case 'S': text_put((RA_Symbol)sym_get_idx(&sym_tbl, symbol)); break;
		case 'A': text_put((RA_Array)sym_get_or_die(&array_tbl, symbol)->u32); break;
		case 'M': text_put((RA_Module)sym_get_idx(&module_tbl, symbol)); break;
		case 'I': text_put((RA_Import)sym_get_idx(&imp_tbl, symbol)); break;
		case 'r':
		{
			const SymbolValue *v = symtbl_find(&label_tbl, symbol);
			if(v) { /* back reference */
				long offset = v->u32 - next_instr_addr;
				if(offset < REF_MIN_OFS || offset > REF_MAX_OFS)
					error("offset %ld is out of range", offset);
				text_put((RA_TextOffset)offset);
			} else /* forward reference */
				ref_new(symbol);
			break;
		}
		default:
			error_symbol(symbol, "symbol expected");
	}
}

void text_write()
{
	storage_write(&text_sect);
}

void label_add(const Symbol *name)
{
	SymbolValue *v = sym_add_uniq(&label_tbl, name);
	v->u32 = text_sect.len;
}

static void label_clear()
{
	symtbl_clear(&label_tbl);
}

static void ref_new(Symbol *label)
{
	Reference *r = mm_alloc(Reference);
	r->name = label;
	r->lineno = yylineno;
	storage_enlarge(&text_sect, sizeof(RA_TextOffset));
	r->base = next_instr_addr;
	r->value = (RA_TextOffset*)storage_current(&text_sect);
	text_put((RA_TextOffset)0); /* temporary offset value */
	SLIST_INSERT_HEAD(&ref_head, r, le);
}

static void ref_delete(Reference *ref)
{
	symbol_delete(ref->name);
	mm_free(Reference, ref);
}

static void resolve(const Reference *ref)
{
	long offset;
	const SymbolValue *v = sym_get_or_die(&label_tbl, ref->name);
	offset = v->u32 - ref->base;
	if(offset < REF_MIN_OFS || offset > REF_MAX_OFS) {
		fprintf(stderr, "line %d, offset %ld: ", ref->lineno, offset);
		error_symbol(ref->name, "label reference is out of range");
	}
	*ref->value = serial((RA_TextOffset)offset);
	if(verbose >= DL_NUDE) {
		symbol_print(ref->name);
		debug_line("  -> %"PRIu32"(%+ld)", v->u32, offset);
	}
}

static void ref_resolve()
{
	if(verbose >= DL_NUDE && !SLIST_EMPTY(&ref_head))
		debug_line("LBL resolving: ");
	while(!SLIST_EMPTY(&ref_head)) {
		Reference *l = SLIST_FIRST(&ref_head);
		SLIST_REMOVE_HEAD(&ref_head, le);
		resolve(l);
		ref_delete(l);
	}
}

static void sub_print(void);
static void label_print(void);
static void var_print(void);

void sub_finish()
{
	if(current_sub) {
		ref_resolve();
		if(verbose >= DL_DUMP) {
			sub_print();
			var_print();
			label_print();
		}
		label_clear();
		var_clear();
		ptbl_sect[current_sub->i].size = serial(sub_len);
	}
}

static void sub_begin(SymbolValue *p)
{
	if(current_sub)
		sub_finish();
	current_sub = p;
	sub_len = 0;
}

void exp_add(const Symbol *name)
{
	SymbolValue *v = sym_index(&exp_tbl, name);
	RMDExport *e = &exp_sect[v->i];
	SymbolValue *p = sym_get_or_die(&proc_tbl, name);
	e->name = serial(sym_add(name));
	e->idx = p->i;
}

void exp_write()
{
	const int size = symtbl_size(&exp_tbl);
	if(fwrite(exp_sect, sizeof(RMDExport), size, output) != size)
		file_write_error();
}

void header_fill()
{
	if(header.name == 0)
		error("module name was not specified");
	header.datac = serial((RA_Data)symtbl_size(&data_tbl));
	header.sizes.exp  = serial((RA_Export)symtbl_size(&exp_tbl));
	header.sizes.ptbl = serial((RA_Proc)symtbl_size(&proc_tbl));
	header.sizes.mtbl = serial((RA_Module)symtbl_size(&module_tbl));
	header.sizes.imp  = serial((RA_Import)symtbl_size(&imp_tbl));
	header.sizes.text = serial((RA_Text)text_sect.len);
	header.sizes.sym  = serial((RA_Symbol)sym_sect.len);
	header.sizes.str  = serial((RA_Array)str_sect.len);
	header.size = serial_32(header.sizes.exp * sizeof(RMDExport)
		+ header.sizes.ptbl * sizeof(RMDProcedure)
		+ header.sizes.mtbl * sizeof(RMDModule)
		+ header.sizes.imp * sizeof(RMDImport)
		+ header.sizes.text + header.sizes.sym + header.sizes.str);
}

static void header_print()
{
	printf("RMD-%d.%d, version %d.%d\n", header.rmd_version.maj,
			header.rmd_version.min, header.version.maj, header.version.min);
}

static void table_print(const char *name, const SymbolTable *tbl,
		void (*print)(const Symbol *s, const SymbolValue *v))
{
	int size = symtbl_size(tbl);
	if(size != 0) {
		printf("#%s(%d): ", name, size);
		symtbl_foreach(tbl, print);
		putchar('\n');
	}
}

static void data_print1(const Symbol *s, const SymbolValue *v)
{
	symbol_print(s);
	printf("(%d)  ", v->i);
}

static void data_print()
{
	table_print("data", &data_tbl, data_print1);
}

static void module_print1(const Symbol *s, const SymbolValue *v)
{
	const RMDModule *m = &module_sect[v->i];
	symbol_print(s);
	printf("-%hhu.%hhu(%u) ", m->version.maj, m->version.min, m->name);

}

static void module_print()
{
	table_print("mtbl", &module_tbl, module_print1);
}

static void imp_print1(const Symbol *s, const SymbolValue *v)
{
	const RMDImport *imp = &imp_sect[v->i];
	printf("'%d' from '%d'(", imp->name, module_sect[imp->module].name);
	symbol_print(s);
	printf(")  ");
}

static void imp_print()
{
	table_print("imp", &imp_tbl, imp_print1);
}

static void ptbl_print1(const Symbol *s, const SymbolValue *v)
{
	const RMDProcedure *p = &ptbl_sect[v->i];
	symbol_print(s);
	printf(" (%u:%u)  ", p->addr, p->size);
}

static void ptbl_print()
{
	table_print("ptbl", &proc_tbl, ptbl_print1);
}

static void var_print1(const Symbol *s, const SymbolValue *v)
{
	symbol_print(s);
	printf("(%d)  ", v->i);
}

static void var_print()
{
	table_print("	VAR", &var_tbl, var_print1);
}

static void text_print()
{
	printf("#text: %u bytes\n", text_sect.len);
}

static void label_print1(const Symbol *s, const SymbolValue *v)
{
	symbol_print(s);
	printf("(%lu)  ", (unsigned long)v->u32);
}

static void label_print()
{
	table_print("	LABELS", &label_tbl, label_print1);
}

static void sub_print()
{
	const RMDProcedure *p = &ptbl_sect[current_sub->i];
	printf("PROC: argc = %d, varc = %d\n", p->argc, p->varc);
}

static void exp_print1(const Symbol *s, const SymbolValue *v)
{
	const RMDExport *e = &exp_sect[v->i];
	symbol_print(s);
	printf("(%d)  ", e->idx);
}

static void exp_print()
{
	table_print("exp", &exp_tbl, exp_print1);
}

static void sym_print1(const Symbol *s, const SymbolValue *v)
{
	symbol_print(s);
	putchar(' ');
}

static void sym_print()
{
	table_print("sym", &sym_tbl, sym_print1);
}

static void str_print()
{
	if(str_sect.len) {
		printf("#str(%d)  ", str_sect.len);
		putchar('\n');
	}
}

void sect_print()
{
	if(verbose < DL_DUMP)
		return;
	header_print();
	exp_print();
	ptbl_print();
	module_print();
	imp_print();
	text_print();
	sym_print();
	data_print();
	str_print();
	printf("%lu bytes written\n",
			(unsigned long)sizeof(RMDHeader) + deserial_32(header.size));
}

void sect_init()
{
	/* symbol may not have an address of 0 */
	sym_add(symbol_store.empty);
}

void sect_finish()
{
	sub_finish();
}

void sect_write()
{
	header_write();
	exp_write();
	ptbl_write();
	module_write();
	imp_write();
	text_write();
	sym_write();
	str_write();
}

