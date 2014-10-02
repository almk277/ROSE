<instruction>\n                end_instruction();
<instruction>[0-9]*            |
<instruction>'.'               |
<instruction>{SYM1}{SYM}*      text_emit_operand(*args++, YSTRING);

<*>#.*                         /* comment until EOL */
<*>[ \t]+                      /* spaces are allowed anywhere */
<*>\n                          error("unexpected end of line");
<<EOF>>                        error("unexpected end of file");

<*>.                           error("'%c': unexpected symbol", *yytext);

%%
static void emit_instruction(int len, char opcode, const char *argstring)
{
	BEGIN instruction;
	instr_start = text_len();
	text_enlarge(len);
	text_emit_opcode(opcode);
	args = argstring;
	debug_line("instruction %s", yytext);
}

static void end_instruction(void)
{
	if(*args && *args != '-')
		error("not enough arguments");
	BEGIN INITIAL;
}

