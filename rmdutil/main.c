#include "rmd.h"
#include <stdio.h>

static const char *filename;
static FILE *file;
static RMDHeader header;

static void help(void)
{
	fputs("Usage: rmdutil [options] filename\n",
			stderr);
}

static void read_header(void)
{
	if(fread(&header, sizeof header, 1, file) != 1) {
		fputs("file read error\n", stderr);
		return;
	}
}

static int check_signature(void)
{
	return header.ident[0] == RMD_H_IDENT1 && header.ident[1] == RMD_H_IDENT2
		&& header.ident[2] == RMD_H_IDENT3 && header.ident[3] == RMD_H_IDENT4;
}

static void info(void)
{
	printf("ROSE version %hhu.%hhu\n",
			header.rmd_ver[0], header.rmd_ver[1]);
	/*printf("module '%s', version %hhu.%hhu\n");*/
}

int main(int argc, char *argv[])
{
	if(argc == 1) {
		help();
		return 0;
	}
	filename = argv[1];
	file = fopen(filename, "r");
	if(!file) {
		fputs("Can not open file\n", stderr);
		return 1;
	}
	read_header();
	if(!check_signature()) {
		fputs("Not RMD file\n", stderr);
		return 1;
	}
	info();
	fclose(file);
	return 0;
}

