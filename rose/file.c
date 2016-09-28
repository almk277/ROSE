#include "file.h"
#include "file_int.h"
#include "file_rose.h"
#include "array.h"
#include "utility.h"
#include <string.h>
#include <stdlib.h> /* malloc/free */

typedef struct scheme {
	string name;
	RD_File *(*open)(const char *name, R_Byte mode);
} scheme;

static const scheme *get_scheme(const RD_ByteArray *path);

RD_File *file_open(const RD_ByteArray *name, R_Byte mode)
{
	const scheme *s;
	const R_Byte *fname;
	char *c_fname;
	int len;
	RD_File *f;

	s = get_scheme(name);

	/* make a zero-terminated string copy without scheme prefix */
	fname = bytearray_data_c(name);
	len = bytearray_length(name) + 1;
	fname += s->name.len;
	len -= s->name.len;;

	c_fname = malloc(len);
	memcpy(c_fname, fname, len);
	c_fname[len] = '\0';

	f = s->open(c_fname, mode);

	free(c_fname);
	return f;
}

void file_close(RD_File *f)
{
	f->op->close(f);
}

R_Word file_read(RD_File *f, struct RD_ByteArray *a, R_Word size)
{
	return f->op->read(f, a, size);
}

R_Word file_write(RD_File *f, const struct RD_ByteArray *a, R_Word size)
{
	return f->op->write(f, a, size);
}


/*** Local files ***/

void file_local_close(RD_File *f)
{
	LocalFile *lf = (LocalFile*)f;
	fclose(lf->file);
}

R_Word file_local_read(RD_File *f, struct RD_ByteArray *a, R_Word size)
{
	LocalFile *lf = (LocalFile*)f;
	R_Word sz = bytearray_length(a);

	if (sz < size)
		size = sz;
	return fread(bytearray_data(a), size, 1, lf->file);
}

R_Word file_local_write(RD_File *f, const struct RD_ByteArray *a, R_Word size)
{
	LocalFile *lf = (LocalFile*)f;
	R_Word sz = bytearray_length(a);
	if (sz < size)
		size = sz;
	return fwrite(bytearray_data_c(a), size, 1, lf->file);
}

static const struct fileops op_local = {
	file_local_close,
	file_local_read,
	file_local_write,
};

static const char *f_mode(R_Byte mode)
{
	switch (mode) {
		case F_R: return "r";
		case F_W: return "w";
		case F_RW: return "r+";
	}

	return NULL;
}

static RD_File *local_open(const char *name, R_Byte mode)
{
	LocalFile *f;
	FILE *file;
	const char *mode_s;

	mode_s = f_mode(mode);
	if (!mode_s)
		return RD_null;

	file = fopen(name, mode_s);
	if(!file)
		return RD_null;

	f = (LocalFile*)heap_alloc(sizeof(LocalFile));
	f->file = file;
	f->obj.op = &op_local;

	return &f->obj;
}

static const struct scheme s_default = {
	STR(""),
	local_open,
};

static const struct scheme s_local = {
	STR("file:"),
	local_open,
};


static const struct scheme s_rose = {
	STR("rose:"),
	file_rose_open,
};


/* Schemes */

static const scheme *const schemes[] = {
	&s_local,
	&s_rose,
};

static const scheme *get_scheme(const RD_ByteArray *path)
{
	int i;
	int path_len = bytearray_length(path);
	const R_Byte *path_str = bytearray_data_c(path);

	for (i = 0; i != sizeof(schemes) / sizeof(schemes[0]); ++i) {
		int prefix_len = schemes[i]->name.len;
		if (prefix_len <= path_len && memcmp(path_str,
								schemes[i]->name.data, prefix_len) == 0)
				return schemes[i];
	}

	return &s_default;
}

