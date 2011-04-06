#include "text.h"
#include "hash.h"
#include "storage.h"
#include "rmd.h"
#include "common.h"
#include "tables.h"
#include "mm.h"
#include "str.h"

enum JumpType {
	JumpNear,
	JumpFar
};

typedef struct Reference {
	char *name;
	enum JumpType type;
	int line;
	uint32_t addr;
	char *oper;
	SLIST_ENTRY(Reference) le;
} Reference;

static Hash proc_hash;   /* procedures */
static Hash label_hash;  /* labels */
static Hash addr_hash;   /* addresses */
static Hash exp_hash;    /* export table */

static RMDProcedure ptbl_sect[256];                         /* #ptbl */
static RMDExport exp_sect[256];                             /* #exp  */
static Storage text_sect = STORAGE_INITIALIZER(text_sect);  /* #text */
static uint32_t addr_sect[256];                             /* #addr */

static MM_DECL(Reference, 10);

static SLIST_HEAD(, Reference) ref_head;

const HashEntry *proc_add(const char *name)
{
	HashEntry *e = hash_add(&proc_hash, name);
	RMDProcedure *p = &ptbl_sect[e->data];
	p->addr = offset;
	return e;
}

int proc_find(const char *name)
{
	return hash_get(&proc_hash, name);
}

static void proc_print1(const HashEntry *ent)
{
	RMDProcedure *p = &ptbl_sect[ent->data];
	printf("%s => %u  ", ent->name, p->addr);
}

void proc_print(void)
{
	printf("#proc(%d): ", proc_hash.count);
	hash_print(&proc_hash, proc_print1);
}

void proc_write(FILE *file)
{
	if(fwrite(ptbl_sect, sizeof(RMDProcedure), proc_hash.count, file)
			!= proc_hash.count)
		file_write_error();
}

size_t proc_length(void)
{
	return proc_hash.count * sizeof(RMDProcedure);
}

void text_add(uint8_t opcode, uint8_t oper)
{
	storage_add_instr(&text_sect, opcode, oper);
	offset += 2;
}

void text_print(void)
{
	printf("#text: %u bytes\n", text_sect.len);
}

void text_write(FILE *file)
{
	storage_write(&text_sect, file);
}

size_t text_length(void)
{
	return text_sect.len;
}

void label_add(const char *name, uint32_t addr)
{
	HashEntry *e = hash_add(&label_hash, name);
	e->data = addr;
}

int label_find(const char *name, uint32_t *addr)
{
	HashEntry *e = hash_find(&label_hash, name);
	if(!e)
		return 0;
	*addr = e->data;
	return 1;
}

void label_clear(void)
{
	hash_clear(&label_hash);
}

int addr_add(const char *label, uint32_t addr)
{
	HashEntry *e;
	int idx = hash_get(&addr_hash, label);
	if(idx != -1)
		return idx;
	e = hash_add(&addr_hash, label);
	addr_sect[e->data] = addr;
	return e->data;
}

static void addr_print1(const HashEntry *ent)
{
	printf("%u  ", ent->data);
}

void addr_print(void)
{
	printf("#addr(%d):  ", addr_hash.count);
	hash_print(&addr_hash, addr_print1);
}

void addr_write(FILE *file)
{
	if(fwrite(addr_sect, 4, addr_hash.count, file) != addr_hash.count)
		file_write_error();
}

size_t addr_length(void)
{
	return addr_hash.count * 4;
}

static void label_print1(const HashEntry *ent)
{
	printf("%s  ", ent->name);
}

void label_print(void)
{
	if(label_hash.count) {
		printf("LABELS[%s](%d):  ", cur_proc->name, label_hash.count);
		hash_print(&label_hash, label_print1);
	}
}

static char *text_current(void)
{
	return storage_current(&text_sect);
}

static void forward_ref(const char *label, enum JumpType type)
{
	Reference *l = mm_alloc(Reference);
	if(!l)
		error("out of memory");
	l->name = str_clone(label);
	l->oper = text_current() + 1;
	l->addr = offset;
	l->type = type;
	l->line = lineno;
	SLIST_INSERT_HEAD(&ref_head, l, le);
}

void forward_ref_near(const char *label)
{
	forward_ref(label, JumpNear);
}

void forward_ref_far(const char *label)
{
	forward_ref(label, JumpFar);
}

static void resolve(const Reference *label)
{
	uint32_t addr;
	uint8_t oper;
	if(!label_find(label->name, &addr))
		error("unknown label '%s'", label->name);
	switch(label->type) {
		case JumpNear:
			{
				int ofs = (addr - label->addr) / 2;
				if(ofs > 127)
					error("too long jump to '%s' at %u",
							label->name, label->line);
				oper = ofs;
				/*debug_line("back near jump: from %u to %u on %d",*/
						/*label->addr, addr*/
				debug_line("LABEL near: '%s' ofs=%u at %u",
						label->name, ofs, label->addr);
				break;
			}
		case JumpFar:
			oper = addr_add(label->name, addr);
			debug_line("LABEL far: '%s' addr=%hhu at %u",
					label->name, oper, label->addr);
			break;
		default:
			error("%s: switch error", __func__);
	}
	*label->oper = oper;
}

void ref_resolve(void)
{
	while(!SLIST_EMPTY(&ref_head)) {
		Reference *l = SLIST_FIRST(&ref_head);
		resolve(l);
		str_free(l->name);
		mm_free(Reference, l);
		SLIST_REMOVE_HEAD(&ref_head, le);
	}
}

void exp_add(const HashEntry *proc)
{
	HashEntry *ent = hash_add(&exp_hash, proc->name);
	RMDExport *e = &exp_sect[ent->data];
	e->name = sym_add(proc->name);
	e->idx = proc->data;
}

static void exp_print1(const HashEntry *ent)
{
	printf("%s(%d)  ", ent->name, ent->data);
}

void exp_print(void)
{
	printf("#exp(%d)  ", exp_hash.count);
	hash_print(&exp_hash, exp_print1);
}

void exp_write(FILE *file)
{
	if(fwrite(exp_sect, sizeof(RMDExport), exp_hash.count, file)
			!= exp_hash.count)
		file_write_error();
}

