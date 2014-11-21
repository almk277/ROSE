<instruction>\n                end_instruction();
<instruction>'\\.'             text_emit_escaped_char(*args++, yytext[2]);
<instruction>'.'               text_emit_char(*args++, yytext[1]);
<instruction>{FLT}             text_emit_flt(*args++, yytext);
<instruction>{INT}             text_emit_int(*args++, yytext);
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
	instr_start = text_addr();
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

