#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "print.h"
#include "sect.h"
#include "mm.h"
#include "cmdopt.h"

static void *mm_error(mmPoolStruct *p)
{
	error("Out of memory");
	return 0;
}

void *(*mm_on_error)(mmPoolStruct *) = mm_error;

int verbose = DL_NONE;
static const char *input_name = 0;
const char *output_name = 0;

FILE *output;
/* from generated ras-lex.c */
extern FILE *yyin;
extern int yylex(void);

static void usage(const char *progname)
{
	fprintf(stdout, "Usage:\n"
			"%s [OPTIONS] FILE.ras\n",
			progname);
	fputs(
			"Options are:\n"
			"  -help        -- this help message\n"
			"  -output=name -- create file 'name' instead of FILE.rmd\n"
#ifdef DEBUG
			"  -verbose[=n] -- verbose mode, n is level (1..3)\n"
#endif
		, stdout);
}

static void parse_cmd(int argc, char *argv[])
{
	int help = 0;
	struct cmdopt opts[] = {
		{ "help", CMDOPT_NONE, &help },
		{ "output", CMDOPT_STR, &output_name },
#ifdef DEBUG
		{ "verbose", CMDOPT_LONG1, &verbose },
#endif
		{ NULL, 0, NULL }
	};

	int file_idx = cmdopt(argc, argv, opts);
	if(help) {
		fputs("ROSE assembler. ", stdout);
		usage(argv[0]);
		exit(1);
	}
	if(file_idx <= 0) {
		if(file_idx == 0)
			printf("%s: no input file specified. ", argv[0]);
		puts("See -help for details."); 
		exit(1);
	}
	input_name = argv[file_idx];
	if(!output_name)
		output_name = "out.rmd";
}

static FILE *fopen_and_check(const char *name, const char *mode)
{
	FILE *f = fopen(name, mode);
	if(!f)
		file_error(name);
	return f;
}

static void fclose_and_check(const char *name, FILE *f)
{
	if(fclose(f) == EOF)
		file_error(name);
}

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "");
	parse_cmd(argc, argv);
	sect_init();

	yyin = fopen_and_check(input_name, "r");
	yylex();
	fclose(yyin); /* ignore errors */

	header_fill();
	sect_print();

	output = fopen_and_check(output_name, "w");
	sect_write();
	fclose_and_check(output_name, output);

	return 0;
}

