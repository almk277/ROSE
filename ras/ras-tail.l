<instruction>\n                end_instruction();
<instruction>{CONST}           text_emit_const(*args++, yytext);
<instruction>{WORD}            text_emit_symbol(*args++, YSTRING);

<*>#.*                         /* comment until EOL */
<*>[ \t]+                      /* spaces are allowed anywhere */
<*>\n                          error("unexpected end of line");
<<EOF>>                        error("unexpected end of file");
<*>.                           error("'%s': unexpected token", yytext);

%%
static void emit_instruction(int len, char opcode, const char *argstring)
{
	BEGIN instruction;
	instr_start = text_len();
	text_enlarge(len);
	sub_len += len;
	text_emit_opcode(opcode);
	args = argstring;
	debug_line("instruction %s", yytext);
}

static void end_instruction()
{
	if(*args && *args != '-')
		error("not enough arguments");
	BEGIN INITIAL;
}

