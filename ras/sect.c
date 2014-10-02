#include "sect.h"
#include "print.h"
#include "hash.h"
#include "storage.h"
#include "rmd.h"
#include "mm.h"
#include <string.h>
#include <stdio.h>
#include <inttypes.h>

/* ROSE version */
#define ROSE_VERSION_MAJ   1
#define ROSE_VERSION_MIN   0

/* all names */
static Hash data_hash;   /* module data */
static Hash module_hash; /* imported modules */
static Hash imp_hash;    /* imported methods */
static Hash ptbl_hash;   /* procedures */
static Hash label_hash;  /* labels */
static Hash exp_hash;    /* export table */
static Hash var_hash;    /* current method variables */
static Hash array_hash;  /* arrays */

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
	String *name;                /* referenced label */
	int lineno;                  /* reference source line */
	uint32_t base;               /* reference base address */
	int16_t *value;              /* reference value address */
	SLIST_ENTRY(Reference) le;   /* list entry */
} Reference;

SLIST_HEAD(RefList, Reference) ref_head;
static MM_DECL(Reference, 16);

HashEntry *current_sub = 0;

RMDHeader header = {
	{
		RMD_H_IDENT1,
		RMD_H_IDENT2,
		RMD_H_IDENT3,
		RMD_H_IDENT4
	},
	{
		ROSE_VERSION_MAJ,
		ROSE_VERSION_MIN
	},
	0,
	0,
	{ 0, 0 }
};

#define text_put1byte(byte) storage_put1byte(&text_sect, byte)
#define text_put2byte(byte) storage_put2byte(&text_sect, byte)
#define text_put4byte(byte) storage_put4byte(&text_sect, byte)

static void check_current_sub(void)
{
	if(!current_sub)
		error("no current procedure");
}

void data_add(const String *name)
{
	hash_add(&data_hash, name);
}

int data_find(const String *name)
{
	return hash_get(&data_hash, name);
}

uint16_t sym_add(const String *sym)
{
	return storage_add_string(&sym_sect, sym);
}

void sym_write(void)
{
	storage_write(&sym_sect);
}

void module_add(const String *name, uint8_t maj, uint8_t min)
{
	HashEntry *e = hash_add(&module_hash, name);
	RMDModule *m = &module_sect[e->data.u8];
	m->name = sym_add(name);
	m->version[0] = maj;
   	m->version[1] = min;
	m->sum = 0;
}

int module_find(const String *name)
{
	return hash_get(&module_hash, name);
}

void module_write(void)
{
	if(fwrite(module_sect, sizeof(RMDModule), module_hash.count, output)
			!= module_hash.count)
		file_write_error();
}

void imp_add(const String *name)
{
	RMDImport *imp;
	HashEntry *ent;
	char *proc;
	int idx;
	String *str;
	int module_len;

	proc = strchr(name->data, ':');
	if(!proc)
		error("import name must be specified as 'module:procedure'");
	module_len = proc - name->data;
 	ent = hash_add(&imp_hash, name);
	*proc++ = 0;

	str = string_new(name->data, module_len);
	idx = module_find(str);
	string_delete(str);
	if(idx == -1)
		error("module '%s' not found", proc);
	imp = &imp_sect[ent->data.u8];
	imp->name = sym_add(name);
	imp->module = idx;
	imp->slot = imp->addr = 0;
}

int imp_find(const String *name)
{
	return hash_get(&imp_hash, name);
}

void imp_write(void)
{
	if(fwrite(imp_sect, sizeof(RMDImport), imp_hash.count, output)
			!= imp_hash.count)
		file_write_error();
}

void header_set_name(const String *name)
{
	if(header.name != 0)
		error("module has name already");
	header.name = sym_add(name);
}

void header_set_parent(const String *name)
{
	if(header.parent != 0)
		error("module is inherited already");
	header.parent = sym_add(name);
}

void header_write(void)
{
	if(fwrite(&header, sizeof(RMDHeader), 1, output) != 1)
		file_write_error();
}

void str_begin(const String *name)
{
	HashEntry *ent = hash_add(&array_hash, name);
	ent->data.u32 = array_begin(&str_sect);
}

void str_add_char(char c)
{
	array_add_byte(c);
}

void str_add_string(const String *str)
{
	array_add_string(str);
}

void str_write(void)
{
	storage_write(&str_sect);
}

void sub_begin(HashEntry *p);

void ptbl_add(const String *name)
{
	HashEntry *e;
	RMDProcedure *p;

	if(ptbl_hash.count == sizeof(ptbl_sect) / sizeof(ptbl_sect[0]))
		error("%s: too many procedures", name->data);
	e = hash_add(&ptbl_hash, name);
	p = &ptbl_sect[e->data.u8];
	p->addr = text_sect.len;
	p->argc = p->varc = 0;
	sub_begin(e);
}

int ptbl_find(const String *name)
{
	return hash_get(&ptbl_hash, name);
}

void ptbl_write(void)
{
	if(fwrite(ptbl_sect, sizeof(RMDProcedure), ptbl_hash.count, output)
			!= ptbl_hash.count)
		file_write_error();
}

void var_add(const String *name)
{
	uint8_t proc_idx;
	check_current_sub();
	hash_add(&var_hash, name);
	proc_idx = current_sub->data.u8;
	++ptbl_sect[proc_idx].varc;
}

int var_find(const String *name)
{
	return hash_get(&var_hash, name);
}

void var_clear(void)
{
	hash_clear(&var_hash);
}

void arg_add(const String *name)
{
	uint8_t proc_idx;
	check_current_sub();
	hash_add(&var_hash, name);
	proc_idx = current_sub->data.u8;
	++ptbl_sect[proc_idx].argc;
}

void text_enlarge(int size)
{
	storage_enlarge(&text_sect, size);
}

char *text_addr(void)
{
	return storage_current(&text_sect);
}

uint32_t text_len(void)
{
	return text_sect.len;
}

void text_emit_opcode(char opcode)
{
	text_put1byte(opcode);
}

static const HashEntry *label_find(const String *name);
static void ref_new(String *label);

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

static long num_parse(const char *string, int code)
{
	long arg;

	if(string[0] == '\'' && string[2] == '\'')
		return string[1];

	if(sscanf(string, num_desc[code].fmt, &arg) != 1)
		error("%s: not a number", string);
	if(arg < num_desc[code].min || arg > num_desc[code].max)
		error("%ld: number out of range", arg);
	return arg;
}

void text_emit_operand(char type, String *string)
{
	char *str = string->data;
	switch(type) {
		case 0:
		case '-':
			error("'%s': superfluous argument", str);
		case 's':
			{
				const HashEntry *ent = hash_find(&var_hash, string);
				if(!ent)
					ent = hash_find(&data_hash, string);
				if(!ent)
					error("%s: variable not found", str);
				text_put1byte(ent->data.u8);
			}
			break;
		case 'b':
			text_put1byte((int8_t)num_parse(str, NUMDESC8));
			break;
		case 'h':
			text_put2byte((int16_t)num_parse(str, NUMDESC16));
			break;
		case 'w':
			text_put4byte((int32_t)num_parse(str, NUMDESC32));
			break;
		case 'p':
			text_put1byte(hash_get_or_die(&ptbl_hash, string));
			break;
		case 'a':
			text_put4byte(hash_get_or_die(&array_hash, string));
			break;
		case 'm':
			text_put1byte(hash_get_or_die(&module_hash, string));
			break;
		case 'i':
			text_put1byte(hash_get_or_die(&imp_hash, string));
			break;
		case 'e':
			text_put1byte(hash_get_or_die(&exp_hash, string));
			break;
		case 'r':
			{
				const HashEntry *ent = label_find(string);
				if(ent) {
					int32_t offset = ent->data.u32 - text_sect.len;
					if(offset < INT16_MIN || offset > INT16_MAX)
						error("offset %" PRIx32 " is out of range", offset);
					text_put2byte(offset);
				} else
					ref_new(string);
				break;
			}
		default:
			error("%s: switch error", __func__);
	}
}

void text_write(void)
{
	storage_write(&text_sect);
}

void label_add(const String *name)
{
	HashEntry *e = hash_add(&label_hash, name);
	e->data.u32 = text_sect.len;
}

static const HashEntry *label_find(const String *name)
{
	return hash_find(&label_hash, name);
}

void label_clear(void)
{
	hash_clear(&label_hash);
}

static void ref_new(String *label)
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
	string_delete(ref->name);
	mm_free(Reference, ref);
}

static void resolve(const Reference *ref)
{
	int32_t offset;
	const HashEntry *ent = label_find(ref->name);
	if(!ent)
		error("unknown label '%s'", ref->name->data);

	offset = ent->data.u32 - ref->base;
	if(offset < REF_MIN_OFS || offset > REF_MAX_OFS)
		error("%s: label reference at line %d is out of range: %d",
				ref->name->data, ref->lineno, offset);
	*ref->value = (int16_t)offset;
	if(verbose >= DL_NUDE) 
		debug_line("  %s -> %"PRIu32"(%+"PRIi16")",
				ref->name->data, ent->data.u32, offset);
}

static void ref_resolve(void)
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

void sub_finish(void)
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

void sub_begin(HashEntry *p)
{
	if(current_sub)
		sub_finish();
	current_sub = p;
}

void exp_add(const String *name)
{
	HashEntry *ent = hash_add(&exp_hash, name);
	RMDExport *e = &exp_sect[ent->data.u8];
	e->name = sym_add(name);
	e->idx = ent->data.u8;
}

void exp_write(void)
{
	if(fwrite(exp_sect, sizeof(RMDExport), exp_hash.count, output)
			!= exp_hash.count)
		file_write_error();
}

void header_fill(void)
{
	if(header.name == 0)
		error("module name was not specified");
	header.exp  = exp_hash.count;
	header.ptbl = ptbl_hash.count;
	header.mtbl = module_hash.count;
	header.imp  = imp_hash.count;
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

static void data_print1(const HashEntry *ent)
{
	printf("%s(%d)  ", ent->string->data, ent->data.u8);
}

static void data_print(void)
{
	printf(".data(%d): ", data_hash.count);
	hash_print(&data_hash, data_print1);
}

static void sym_print(void)
{
    printf("#sym: ");
    storage_print_str(&sym_sect);
}

static void module_print1(const HashEntry *ent)
{
	const RMDModule *m = &module_sect[ent->data.u8];
	printf("%s-%hhu.%hhu(%u)[%u] ", ent->string->data,
			m->version[0], m->version[1], m->name, m->sum);

}

static void module_print(void)
{
	printf("#mtbl(%d): ", module_hash.count);
	hash_print(&module_hash, module_print1);
}

static void imp_print1(const HashEntry *ent)
{
	const RMDImport *imp = &imp_sect[ent->data.u8];
	printf("'%d' from '%d'(%s)  ", imp->name,
			module_sect[imp->module].name, ent->string->data);
}

static void imp_print(void)
{
	printf("#imp(%d):  ", imp_hash.count);
	hash_print(&imp_hash, imp_print1);
}

static void header_print(void)
{
	printf("RMD-%d.%d, version %d.%d\n", header.rmd_ver[0],
			header.rmd_ver[1], header.version[0], header.version[1]);
}

static void ptbl_print1(const HashEntry *ent)
{
	RMDProcedure *p = &ptbl_sect[ent->data.u8];
	printf("%s => %u  ", ent->string->data, p->addr);
}

static void ptbl_print(void)
{
	printf("#ptbl(%d): ", ptbl_hash.count);
	hash_print(&ptbl_hash, ptbl_print1);
}

static void var_print1(const HashEntry *ent)
{
	printf("%s(%d)  ", ent->string->data, ent->data.u8);
}

static void var_print(void)
{
	if(var_hash.count) {
		printf("VAR[%s](%d):  ", current_sub->string->data, var_hash.count);
		hash_print(&var_hash, var_print1);
	}
}

static void text_print(void)
{
	printf("#text: %u bytes\n", text_sect.len);
}

static void label_print1(const HashEntry *ent)
{
	printf("%s  ", ent->string->data);
}

static void label_print(void)
{
	if(label_hash.count) {
		printf("LABELS[%s](%d):  ", current_sub->string->data, label_hash.count);
		hash_print(&label_hash, label_print1);
	}
}

static void sub_print(void)
{
	RMDProcedure *p = &ptbl_sect[current_sub->data.u8];
	printf("PROC END(%s): argc = %d, varc = %d\n",
			current_sub->string->data, p->argc, p->varc);
}

static void exp_print1(const HashEntry *ent)
{
	printf("%s(%d)  ", ent->string->data, ent->data.u8);
}

static void exp_print(void)
{
	printf("#exp(%d)  ", exp_hash.count);
	hash_print(&exp_hash, exp_print1);
}
 
static void str_print(void)
{
	printf("#str(%d)  ", str_sect.len);
	putchar('\n');
}

void sect_print(void)
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

void sect_init(void)
{
	/* symbol may not have an address of 0 */
	String *empty = string_new("", 1);
	sym_add(empty);
	string_delete(empty);
}

void sect_finish(void)
{
	sub_finish();
}

void sect_write(void)
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

