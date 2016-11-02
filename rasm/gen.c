/* Code generator for rasm */

#include <stdio.h>
#include <stdlib.h>

const char oper_name[] = "../doc/operand.dsc";
const char instr_name[] = "../doc/isa.dsc";
const char out_name[] = "instr.cpp";

const char head[] = "#include \"instr.hpp\"\n\n"
"extern const std::unordered_map<std::string, instruction> instr = {\n";
const char tail[] = "};";

int lens[256];

FILE *safe_open(const char *name, const char *mode)
{
	FILE *f = fopen(name, mode);
	if (!f) {
		perror(name);
		exit(1);
	}
	return f;
}

void safe_close(const char *name, FILE *f)
{
	if (fclose(f) == EOF) {
		perror(name);
		exit(1);
	}
}

int oplen(char op)
{
	if (lens[op] < 0) {
		fprintf(stderr, "unknown operand: %c\n", op);
		exit(1);
	}
	return lens[op];
}

int instr_len(const char *args)
{
	const char *c;
	int len = 1; /* opcode length is 1 */

	for (c = args; *c; ++c)
		len += oplen(*c);

	return len;
}

void read_file(const char *fname, void (*fn)(FILE *f, void *ctx), void *ctx)
{
	FILE *in = safe_open(fname, "r");

	for (;;) {
		int c = fgetc(in);

		if (c == EOF)
			break;
		if (c == '\n')
			continue;
		if (c == '#')
			goto nextline;

		ungetc(c, in);
		(*fn)(in, ctx);
nextline:
		do {
			c = fgetc(in);
		} while (c != '\n' && c != EOF);
	}
	fclose(in);
}

void init_operands(void)
{
	int i;
	for (i = 0; i != 256; ++i)
		lens[i] = -1;
}

void read_operands(FILE *in, void *ctx)
{
	char op;
	int len;

	(void)ctx;
	if (fscanf(in, "%c %d", &op, &len) != 2) {
		fprintf(stderr, "operand parse error near byte %ld\n", ftell(in));
		exit(1);
	}
	if (lens[op] != -1) {
		fprintf(stderr, "'%c': operand duplicated\n", op);
		exit(1);
	}
	lens[op] = len;
}

void write_table(FILE *in, void *ctx)
{
	FILE *out = ctx;
	char name[16], args[8], code[3];

	if (fscanf(in, "%15s %7s %2s", name, args, code) != 3) {
		fprintf(stderr, "instruction parse error near byte %ld\n", ftell(in));
		exit(1);
	}
	fprintf(out, "\t{ \"%s\", { 0x%s, %d, \"%s\" } },\n",
			name, code, instr_len(args), args);
}

int main(void)
{
	FILE *out;

	init_operands();
	read_file(oper_name, read_operands, NULL);

	out = safe_open(out_name, "w");
	fputs(head, out);
	read_file(instr_name, write_table, out);
	fputs(tail, out);
	safe_close(out_name, out);

	return 0;
}

