#ifndef ROSE_FILE_INT_H
#define ROSE_FILE_INT_H

#include "file.h"
#include "heap.h"
#include <stdio.h>

struct fileops {
	void (*close)(RD_File *f);
	R_Word (*read)(RD_File *f, struct RD_ByteArray *a, R_Word size);
	R_Word (*write)(RD_File *f, const struct RD_ByteArray *a, R_Word size);
};

struct RD_File {
	RefData refdata;
	const struct fileops *op;
};

typedef struct LocalFile {
	RD_File obj;
	FILE *file;
} LocalFile;

void file_local_close(RD_File *f);
R_Word file_local_read(RD_File *f, struct RD_ByteArray *a, R_Word size);
R_Word file_local_write(RD_File *f, const struct RD_ByteArray *a, R_Word size);

#endif

