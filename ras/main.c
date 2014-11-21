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
const char *output_name = "out.rmd";

FILE *output;
/* from generated ras-lex.c */
extern FILE *yyin;
extern int yylex(void);

static void usage(const char *progname, const struct cmdopt opts[])
{
	puts("ROSE assembler. Usage:");
	printf("%s [OPTIONS] <filename>\n", progname);
	puts("Options are:");
	cmdopt_print(opts);
}

static void set_input(const char *name)
{
	input_name = name;
}

static void set_output(const char *name)
{
	if(!name) {
		puts("output name omitted");
		exit(1);
	}
	output_name = name;
}

static void set_verbose(const char *v)
{
	if(v) {
		if(sscanf(v, "%d", &verbose) != 1 || verbose < 0 || verbose > 3) {
			puts("-verbose: integer [0..3] expected");
			exit(1);
		}
	} else
		verbose = 1;
}

static void parse_cmd(int argc, char *argv[])
{
	int help = 0;
	const struct cmdopt opts[] = {
		{ "help", &help, NULL, "this help message" },
		{ "output", NULL, set_output, "output file name ('out.rmd' by default)" },
		{ "verbose", NULL, set_verbose, "verbosity level (0..3)" },
		{ NULL, NULL, set_input }
	};

	int err = cmdopt_parse(argc, argv, opts);
	if(err) {
		printf("unknown option: %s\n", argv[err]);
		exit(1);
	}
	if(help) {
		usage(argv[0], opts);
		exit(0);
	}
	if(!input_name) {
		printf("%s: no input file specified. ", argv[0]);
		puts("See -help for details."); 
		exit(1);
	}
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
	/* LC_ALL influences scanf badly */
	/*setlocale(LC_MESSAGES, "");*/
	parse_cmd(argc, argv);
	sect_init();

	yyin = fopen_and_check(input_name, "r");
	yylex();
	fclose(yyin); /* ignore errors */

	header_fill();
	sect_print();

	output = fopen_and_check(output_name, "wb");
	sect_write();
	fclose_and_check(output_name, output);

	return 0;
}

