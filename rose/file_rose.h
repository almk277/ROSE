#ifndef ROSE_FILE_ROSE_H
#define ROSE_FILE_ROSE_H

#include "rmd.h"

struct RD_File;

struct RD_File *file_rose_open(const char *name, R_Byte mode);

#endif

