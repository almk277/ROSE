#include "file_rose.h"
#include "file_int.h"
#include "utility.h"
#include <string.h>

static RD_File *stdio_open(const char *name, FILE *file);

static RD_File *open_stdin(const char *name)
{
	return stdio_open(name, stdin);
}

static RD_File *open_stdout(const char *name)
{
	return stdio_open(name, stdout);
}

static RD_File *open_stderr(const char *name)
{
	return stdio_open(name, stderr);
}

typedef struct File {
   string name;
   enum FileMode mode;
   RD_File *(*open)(const char *name);
} File;

static const File rose_f[] = {
	{ STR("/stdin"),  F_R, open_stdin },
	{ STR("/stdout"), F_W, open_stdout },
	{ STR("/stderr"), F_W, open_stderr },
	{ STR(""),        0,   NULL },
};

RD_File *file_rose_open(const char *name, R_Byte mode)
{
	int len;
	const File *f;

	len = strlen(name);
	for (f = rose_f; f->open; ++f)
		if (f->name.len  == len && memcmp(f->name.data, name, len) == 0)
			return (f->mode & mode) ? f->open(name) : RD_null;

	return RD_null;
}

static void std_close(RD_File *f)
{
		(void)f;
}

static const struct fileops op_stdio = {
		std_close,
		file_local_read,
		file_local_write,
};

static RD_File *stdio_open(const char *name, FILE *file)
{
	LocalFile *f;

	f = (LocalFile*)heap_alloc(sizeof(LocalFile));
	f->file = file;
	f->obj.op = &op_stdio;

	return &f->obj;
}

