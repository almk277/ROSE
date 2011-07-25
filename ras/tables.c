#include "tables.h"
#include "common.h"
#include "hash.h"
#include "storage.h"
#include "rmd.h"
#include <string.h>

static Hash const_hash;
static Hash data_hash;
static Hash var_hash;
static Hash module_hash;
static Hash imp_hash;

static RMDModule module_sect[256];
static RMDImport imp_sect[256];
static int32_t const_sect[256];
static Storage sym_sect = STORAGE_INITIALIZER(sym_sect);

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
	{ 0, 0 }
};

void const_add(const char *name, int32_t val)
{
	HashEntry *e = hash_add(&const_hash, name);
	const_sect[e->data] = val;
}

int const_find(const char *name)
{
	return hash_get(&const_hash, name);
}

static void const_print1(const HashEntry *ent)
{
	printf("%s(%d)=%d  ", ent->name, ent->data, const_sect[ent->data]);
}

void const_print(void)
{
	printf("#const(%d): ", const_hash.count);
	hash_print(&const_hash, const_print1);
}

void const_write(FILE *file)
{
	if(fwrite(const_sect, 4, const_hash.count, file) != const_hash.count)
		file_write_error();
}

size_t const_length(void)
{
	return const_hash.count * 4;
}

void data_add(const char *name)
{
	hash_add(&data_hash, name);
}

int data_find(const char *name)
{
	return hash_get(&data_hash, name);
}

static void data_print1(const HashEntry *ent)
{
	printf("%s(%d)  ", ent->name, ent->data);
}

void data_print(void)
{
	printf(".data(%d): ", data_hash.count);
	hash_print(&data_hash, data_print1);
}

uint8_t data_count(void)
{
	return data_hash.count;
}

uint16_t sym_add(const char *sym)
{
	return storage_add_str(&sym_sect, sym);
}

void sym_print(void)
{
    printf("#sym: ");
    storage_print_str(&sym_sect);
}

void sym_write(FILE *file)
{
	storage_write(&sym_sect, file);
}

size_t sym_length(void)
{
	return sym_sect.len;
}

void var_add(const char *name)
{
	hash_add(&var_hash, name);
}

int var_find(const char *name)
{
	return hash_get(&var_hash, name);
}

void var_clear(void)
{
	hash_clear(&var_hash);
}

static void var_print1(const HashEntry *ent)
{
	printf("%s(%d)  ", ent->name, ent->data);
}

void var_print(void)
{
	if(var_hash.count) {
		printf("VAR[%s](%d):  ", cur_proc->name, var_hash.count);
		hash_print(&var_hash, var_print1);
	}
}

void module_add(const char *name, uint8_t maj, uint8_t min)
{
	HashEntry *e = hash_add(&module_hash, name);
	RMDModule *m = &module_sect[e->data];
	m->name = sym_add(name);
	m->version[0] = maj;
   	m->version[1] = min;
	m->sum = 0;
}

int module_find(const char *name)
{
	return hash_get(&module_hash, name);
}

static void module_print1(const HashEntry *ent)
{
	const RMDModule *m = &module_sect[ent->data];
	printf("%s-%hhu.%hhu(%u)[%u] ", ent->name,
			m->version[0], m->version[1], m->name, m->sum);

}

void module_print(void)
{
	printf("#mtbl(%d): ", module_hash.count);
	hash_print(&module_hash, module_print1);
}

void module_write(FILE *file)
{
	if(fwrite(module_sect, sizeof(RMDModule), module_hash.count, file)
			!= module_hash.count)
		file_write_error();
}

size_t module_length(void)
{
	return module_hash.count * sizeof(RMDModule);
}

RMDImport *imp_add(char *mangl_name)
{
	RMDImport *imp;
	HashEntry *ent = hash_add(&imp_hash, mangl_name);
	char *dog = strchr(mangl_name, '@');
	int idx;
	if(!dog)
		error("name must be specified as 'procedure@module'");
	*dog++ = 0;
	check_name(mangl_name);
	imp = &imp_sect[ent->data];
	imp->name = sym_add(mangl_name);
	idx = module_find(dog);
	if(idx == -1)
		error("module '%s' not found", dog);
	imp->module = idx;
	imp->slot = imp->addr = 0;
	return imp;
}

int imp_find(const char *mangl_name)
{
	return hash_get(&imp_hash, mangl_name);
}

static void imp_print1(const HashEntry *ent)
{
	const RMDImport *imp = &imp_sect[ent->data];
	printf("'%d' from '%d'(%s)  ", imp->name, module_sect[imp->module].name, ent->name);
}

void imp_print(void)
{
	printf("#imp(%d):  ", imp_hash.count);
	hash_print(&imp_hash, imp_print1);
}

void imp_write(FILE *file)
{
	if(fwrite(imp_sect, sizeof(RMDImport), imp_hash.count, file)
			!= imp_hash.count)
		file_write_error();
}

size_t imp_length(void)
{
	return imp_hash.count * sizeof(RMDImport);
}

void header_print(void)
{
	printf("RMD-%d.%d, version %d.%d\n", header.rmd_ver[0],
			header.rmd_ver[1], header.version[0], header.version[1]);
}

void header_write(FILE *file)
{
	if(fwrite(&header, sizeof(RMDHeader), 1, file) != 1)
		file_write_error();
}

size_t str_length(void)
{
	return 0;
}

