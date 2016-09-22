#include "file.h"
#include "array.h"
#include "object.h"
#include <stdio.h>
#include <string.h>

struct RD_File {
	RefData refdata;
	FILE *file;
};

static const char *f_mode(R_Byte mode)
{
	switch (mode) {
		case 0: return "r";
		case 1: return "w";
	}

	return "r+";
}

RD_File *file_open(struct RD_ByteArray *name, R_Byte mode)
{
	RD_File *f;
	char c_name[512];
	size_t name_len;
	FILE *file;

	name_len = bytearray_length(name);
	if (name_len > sizeof c_name)
		return RD_null;
	memcpy(c_name, bytearray_data(name), name_len);
	c_name[name_len] = '\0';

	file = fopen(c_name, f_mode(mode));
	if(!file)
		return RD_null;

	f = (RD_File*)heap_alloc(sizeof(RD_File));
	f->file = file;

	return f;
}

void file_close(RD_File *f)
{
	fclose(f->file);
}

R_Word file_read(RD_File *f, RD_ByteArray *a, R_Word size)
{
	R_Word sz = bytearray_length(a);
	if (sz < size)
		size = sz;
	return fread(bytearray_data(a), size, 1, f->file);
}

R_Word file_write(RD_File *f, RD_ByteArray *a, R_Word size)
{
	R_Word sz = bytearray_length(a);
	if (sz < size)
		size = sz;
	return fwrite(bytearray_data(a), size, 1, f->file);
}

