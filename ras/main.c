#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "common.h"
#include "direct.h"
#include "section.h"
#include "tables.h"
#include "proc.h"
#include "text.h"

#define MAX_WORD_LEN            16
#define COMMENT_SIGN            '#'
#define DEFAULT_OUT_FILE_NAME   "out.rmd"
#define MAX_LINE_LEN      128

static FILE *input;
static FILE *output;

static const char *out_file_name = DEFAULT_OUT_FILE_NAME;

static void out_help(void)
{
	fputs(
			"ROSE assembler\n"
			"-h -- this help message\n"
			"-o file_name -- place output in file 'file_name'\n"
			"-v -- verbose mode. Might be used multiple times\n"
		, stdout);
}

static void cmd_parse(int argc, char *argv[])
{
	int opt;
	while((opt = getopt(argc, argv, "o:vh")) != -1) {
		switch(opt) {
			case 'v':
				++verbose;
				break;
			case 'h':
				out_help();
				exit(0);
			case 'o':
				out_file_name = optarg;
				break;
		}
	}
	if(argc <= optind || !strcmp(argv[optind], "-"))
		input = stdin;
	else {
		const char *inname = argv[optind];
		input = fopen(inname, "r");
		if(!input) {
			perror(inname);
			exit(1);
		}
	}
	output = fopen(out_file_name, "w");
	if(!output)
		error("can not create file %s", out_file_name);
}

static void finish(void)
{
	fclose(output);
	fclose(input);
}

static void parse_line(char *line)
{
	char keyword[MAX_WORD_LEN + 1];
	char *space = line;
	int keylen;
	go_to_space(&space);
	keylen = space - line;
	if(keylen > MAX_WORD_LEN)
		error("too long word");
	strncpy(keyword, line, keylen);
	keyword[keylen] = '\0';
	while(isspace(*space))
		++space;
	if(keyword[0] == '.')
		read_direct(keyword + 1, space);
	else
		in_section(keyword, space);
}

static void read_source()
{
	static char buf[MAX_LINE_LEN];
	while(fgets(buf, sizeof buf, input)) {
		char *start;
		char *comment, *newline;
		++lineno;
		comment = strchr(buf, COMMENT_SIGN);
		if(comment)
			*comment = '\0';
		newline = strchr(buf, '\n');
		if(newline)
			*newline = 0;
		start = buf;
		while(isspace(*start))
			++start;
		if(*start == '\0')
			continue;
		parse_line(start);
	}
	proc_finish();
}

static void debug_print(void)
{
	header_print();
	exp_print();
	ptbl_print();
	module_print();
	imp_print();
	const_print();
	addr_print();
	text_print();
	sym_print();
	data_print();
	str_print();
	printf("%d bytes written\n", sizeof(RMDHeader) + header.size);
}

static void make_header(void)
{
	if(header.name == 0)
		error("module name was not specified");
	header.exp  = exp_count();
	header.ptbl = ptbl_count();
	header.mtbl = module_count();
	header.imp  = imp_count();
	header.cnst = const_count();
	header.addr = addr_count();
	header.text = text_count();
	header.sym  = sym_count();
	header.str  = str_count();
	header.size = header.exp * sizeof(RMDExport)
		+ header.ptbl * sizeof(RMDProcedure) + header.mtbl * sizeof(RMDModule)
		+ header.imp * sizeof(RMDImport) + 4 * (header.addr + header.cnst)
		+ header.text + header.sym + header.str;
	header.debug = 0;
}

static void write_rmd(void)
{
	header_write(output);
	exp_write(output);
	ptbl_write(output);
	module_write(output);
	imp_write(output);
	const_write(output);
	addr_write(output);
	text_write(output);
	sym_write(output);
	str_write(output);
}

int main(int argc, char *argv[])
{
	cmd_parse(argc, argv);
	sym_add("");
	read_source();
	make_header();
	if(verbose >= DL_DUMP)
		debug_print();
	write_rmd();
	finish();
	return 0;
}

