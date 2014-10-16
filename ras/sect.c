#include "sect.h"
#include "print.h"
#include "symtbl.h"
#include "storage.h"
#include "rmd.h"
#include "mm.h"
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
extern uint32_t instr_start;

#define REF_SIZE  2
#define REF_MIN_OFS INT16_MIN
#define REF_MAX_OFS INT16_MAX
typedef struct Reference {
	Symbol *name;                /* referenced label */
	int lineno;                  /* reference source line */
	uint32_t base;               /* reference base address */
	int16_t *value;              /* reference value address */
	SLIST_ENTRY(Reference) le;   /* list entry */
} Reference;

static SLIST_HEAD(RefList, Reference) ref_head;
static MM_DECL(Reference, 16);

static SymbolValue *current_sub = 0;
static RMDModule *current_import;

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

#define text_put1byte(byte) storage_put1byte(&text_sect, byte)
#define text_put2byte(byte) storage_put2byte(&text_sect, byte)
#define text_put4byte(byte) storage_put4byte(&text_sect, byte)

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

uint16_t sym_add(const Symbol *sym)
{
	return storage_add_symbol(&sym_sect, sym);
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
	current_import->name = sym_add(name);
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
	imp->name = sym_add(proc);
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
	header.name = sym_add(name);
}

void header_set_version(const char *version)
{
	parse_version(version, &header.version);
}

void header_set_parent(const Symbol *name)
{
	if(header.parent != 0)
		error("module is inherited already");
	header.parent = sym_add(name);
}

void header_write()
{
	if(fwrite(&header, sizeof(RMDHeader), 1, output) != 1)
		file_write_error();
}

void str_begin(const Symbol *name)
{
	SymbolValue *v = sym_index(&array_tbl, name);
	v->u32 = array_begin(&str_sect);
}

void str_add_char(char c)
{
	array_add_byte(c);
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
	p->addr = text_sect.len;
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

void text_enlarge(int size)
{
	storage_enlarge(&text_sect, size);
}

char *text_addr()
{
	return storage_current(&text_sect);
}

uint32_t text_len()
{
	return text_sect.len;
}

void text_emit_opcode(char opcode)
{
	text_put1byte(opcode);
}

static void ref_new(Symbol *label);

#define NUMDESC8  0
#define NUMDESC16 1
#define NUMDESC32 2
static struct NumberDesc {
	const char *fmt;
	long min;
	long max;
} num_desc[] = {
	{ "%"PRIi8,  INT8_MIN,  INT8_MAX },
	{ "%"PRIi16, INT16_MIN, INT16_MAX },
	{ "%"PRIi32, INT32_MIN, INT32_MAX },
};

static char sym2num(char sym)
{
	switch(sym) {
		case 'n':  return '\n';
		case 't':  return '\t';
		case '\\': return '\\';
		case '\'': return '\'';
		default: return sym;
	}
}

static long num_parse(const char *string, int code)
{
	long arg;

	if(string[0] == '\'') {
	   	if(string[2] == '\'')
			return sym2num(string[1]);
		else if(string[1] == '\\' && string[3] == '\'')
			return sym2num(string[2]);
		else
			error("%s: can not parse", string);
	}

	if(sscanf(string, num_desc[code].fmt, &arg) != 1)
		error("%s: not a number", string);
	if(arg < num_desc[code].min || arg > num_desc[code].max)
		error("%ld: number out of range", arg);
	return arg;
}

void text_emit_const(char type, const char *c)
{
	switch(type) {
		case 'c':
			text_put1byte((int8_t)num_parse(c, NUMDESC8)); break;
		case 'w':
			text_put4byte((int32_t)num_parse(c, NUMDESC32)); break;
		default:
			error("%s: numeric constant expected", c);
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
		case 'o': text_put1byte(sym_get_idx(&var_tbl, symbol)); break;

		case 'D': text_put1byte(sym_get_idx(&data_tbl, symbol)); break;
		case 'P': text_put1byte(sym_get_idx(&proc_tbl, symbol)); break;
		case 'S': text_put2byte(0); break; /* TODO */
		case 'A': text_put4byte(sym_get_or_die(&array_tbl, symbol)->u32); break;
		case 'M': text_put1byte(sym_get_idx(&module_tbl, symbol)); break;
		case 'I': text_put1byte(sym_get_idx(&imp_tbl, symbol)); break;
		case 'r':
			{
				const SymbolValue *v = symtbl_find(&label_tbl, symbol);
				if(v) {
					int32_t offset = v->u32 - text_sect.len;
					if(offset < INT16_MIN || offset > INT16_MAX)
						error("offset %" PRIx32 " is out of range", offset);
					text_put2byte(offset);
				} else
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
	storage_enlarge(&text_sect, REF_SIZE);
	text_put2byte(0);
	r->base = instr_start;
	r->value = (int16_t*)text_addr();
	SLIST_INSERT_HEAD(&ref_head, r, le);
}

static void ref_delete(Reference *ref)
{
	symbol_delete(ref->name);
	mm_free(Reference, ref);
}

static void resolve(const Reference *ref)
{
	int32_t offset;
	const SymbolValue *v = sym_get_or_die(&label_tbl, ref->name);
	offset = v->u32 - ref->base;
	if(offset < REF_MIN_OFS || offset > REF_MAX_OFS) {
		fprintf(stderr, "line %d, offset %d: ", ref->lineno, offset);
		error_symbol(ref->name, "label reference is out of range");
	}
	*ref->value = (int16_t)offset;
	if(verbose >= DL_NUDE) {
		symbol_print(ref->name);
		debug_line("  -> %"PRIu32"(%+"PRIi16")", v->u32, offset);
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
#ifdef DEBUG
		if(verbose >= DL_DUMP) {
			var_print();
			label_print();
			sub_print();
		}
#endif
		label_clear();
		var_clear();
	}
}

static void sub_begin(SymbolValue *p)
{
	if(current_sub)
		sub_finish();
	current_sub = p;
}

void exp_add(const Symbol *name)
{
	SymbolValue *v = sym_index(&exp_tbl, name);
	RMDExport *e = &exp_sect[v->i];
	e->name = sym_add(name);
	e->idx = v->i;
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
	header.exp  = symtbl_size(&exp_tbl);
	header.ptbl = symtbl_size(&proc_tbl);
	header.mtbl = symtbl_size(&module_tbl);
	header.imp  = symtbl_size(&imp_tbl);
	header.text = text_sect.len;
	header.sym  = sym_sect.len;
	header.str  = str_sect.len;
	header.size = header.exp * sizeof(RMDExport)
		+ header.ptbl * sizeof(RMDProcedure) + header.mtbl * sizeof(RMDModule)
		+ header.imp * sizeof(RMDImport)
		+ header.text + header.sym + header.str;
	header.debug = 0;
}

#ifdef DEBUG
/* A lot of debug output */

static void header_print()
{
	printf("RMD-%d.%d, version %d.%d\n", header.rmd_version.maj,
			header.rmd_version.min, header.version.maj, header.version.min);
}

static void table_print(const char *name, const SymbolTable *tbl,
		void (*print)(const Symbol *s, const SymbolValue *v))
{
	printf("#%s(%d): ", name, symtbl_size(tbl));
	symtbl_foreach(tbl, print);
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
	RMDProcedure *p = &ptbl_sect[v->i];
	symbol_print(s);
	printf(" => %u  ", p->addr);
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
	table_print("VAR", &var_tbl, var_print1);
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
	table_print("LABELS", &label_tbl, label_print1);
}

static void sub_print()
{
	RMDProcedure *p = &ptbl_sect[current_sub->i];
	printf("PROC END: argc = %d, varc = %d\n", p->argc, p->varc);
}

static void exp_print1(const Symbol *s, const SymbolValue *v)
{
	symbol_print(s);
	printf("(%d)  ", v->i);
}

static void exp_print()
{
	table_print("exp", &exp_tbl, exp_print1);
}

static void sym_print()
{
    printf("#sym: ");
    storage_print_str(&sym_sect);
}

static void str_print()
{
	printf("#str(%d)  ", str_sect.len);
	putchar('\n');
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
	printf("%d bytes written\n", sizeof(RMDHeader) + header.size);
}

#endif /* DEBUG */

void sect_init()
{
	/* symbol may not have an address of 0 */
	Symbol *empty = symbol_new("", 1);
	sym_add(empty);
	symbol_delete(empty);
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

