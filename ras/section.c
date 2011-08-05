#include "common.h"
#include "tables.h"
#include "instr.h"
#include "proc.h"
#include "text.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

static void in_sect_none(char *keyword, char *args);
static void in_sect_proc(char *keyword, char *args);
static void in_sect_const(char *keyword, char *args);
static void in_sect_str(char *keyword, char *agrs);
static void in_sect_data(char *keyword, char *args);

enum SectionType cur_sect = SECT_NONE;

/* must be synced with enum SectionType */
static void (*section_parse[])(char *, char*) = {
	in_sect_none,
	in_sect_proc,
	in_sect_const,
	in_sect_str,
	in_sect_data,
};

void in_section(char *keyword, char *args)
{
	section_parse[cur_sect](keyword, args);
}

static void in_sect_none(char *keyword, char *args)
{
	error("no current section");
}

static void in_sect_proc(char *keyword, char *args)
{
	const struct RoseInstruction *in;
	char *tail;
	uint8_t oper;
	size_t colon = strlen(keyword) - 1;
	if(keyword[colon] == ':') {
		if(*args)
			unexpect_sym("after label");
		keyword[colon] = '\0';
		label_add(keyword, offset);
		debug_line("label '%s'", keyword);
		return;
	}
	in = rose_instr_find(keyword, strlen(keyword));
	if(!in)
		error("unknown instruction '%s'", keyword);
	tail = args;
	check_word_is_last(args, "after insruction");
	/*go_to_space(&tail);*/
	/*pass_spaces(&tail);*/
	/*if(*tail)*/
		/*unexpect_sym("after instruction");*/
	oper = proc_read(in->operand, args);
	text_add(in->opcode, oper);
	debug_line(".text: %s -> '%s'", keyword, args);
}

static void in_sect_data(char *keyword, char *args)
{
	check_name(keyword);
	check_word_is_last(args, "after data name");
	/*pass_spaces(&args);*/
	/*if(*args)*/
		/*unexpect_sym("after data name");*/
	data_add(keyword);
	debug_line(".data: %s", keyword);
}

static int escaped_char(int ch)
{
	switch(ch) {
		case 'n':
			return '\n';
		case 't':
			return '\t';
		case '0':
			return '\0';
	}
	return ch;
}

static long parse_byte(char **pos)
{
	long val;
	if(**pos == '\'') {
		/* escaped char */
		int escaped = (*pos)[1] == '\\';
		if(escaped)
			++*pos;
		if((*pos)[1] == 0)
			error("character expected");
		val = escaped ? escaped_char((*pos)[1]) : (*pos)[1];
		if((*pos)[2] != '\'')
			error("closing ' expected");
		*pos += 3;
	} else if(isalpha(**pos)) {
		/* name */
		uint32_t addr;
		char *tail = *pos;
		go_to_word_end(&tail);
		if(*tail)
			*tail++ = 0;
		addr = str_find(*pos);
		if(addr == (uint32_t)-1)
			error("string name '%s' not found", *pos);
		val = addr;
	} else {
		/* number */
		char *end;
		val = strtol(*pos, &end, 0);
		if(end == *pos)
			error("number expected");
		if(val < INT32_MIN || val > INT32_MAX)
			error("constant value out of range");
		*pos = end;
	}
	return val;
}

static void in_sect_const(char *keyword, char *args)
{
	long val;
	check_name(keyword);
	if(*args++ != '=')
		error("'=' expected after '%s'", keyword);
	pass_spaces(&args);
	val = parse_byte(&args);
	check_word_is_last(args, "after constant");
	/*pass_spaces(&args);*/
	/*if(*args)*/
		/*unexpect_sym("after constant");*/
	const_add(keyword, val);
	debug_line(".const: %s -> %ld", keyword, val);
}

static void in_sect_str(char *keyword, char *args)
{
	char byte;
	long val = parse_byte(&keyword);
	if(val < 0 || val > 255)
		error("byte is out of range");
	byte = (char)val;
	str_add_char(array_len, byte);
	if(*args)
		unexpect_sym("after constant");
	debug_line(".str: %hhd", byte);
}

