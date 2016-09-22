#ifndef ROSE_FILE_H
#define ROSE_FILE_H

#include "rmd.h"
struct RD_ByteArray;

typedef struct RD_File RD_File;

RD_File *file_open(struct RD_ByteArray *name, R_Byte mode);

void file_close(RD_File *file);

R_Word file_read(RD_File *f, struct RD_ByteArray *a, R_Word size);

R_Word file_write(RD_File *f, struct RD_ByteArray *a, R_Word size);

#endif

