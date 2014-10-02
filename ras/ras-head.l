%top{
#include "print.h"
#include "sect.h"
#include "cstring.h"
#define YSTRING string_new(yytext, yyleng + 1)
#define dl debug_line
static void emit_instruction(int len, char opcode, const char *argstring);
static void end_instruction(void);
static const char *args;
uint32_t instr_start;
}

SYM    [a-zA-Z0-9_]
SYM1   [a-zA-Z_]
WORD   {SYM1}{SYM}*

%option yylineno nounput noyywrap
%x instruction eol sub arg var exp imp data mod inh str strsect string

%%

\n             /* nothing special */
<INITIAL><<EOF>>     sect_finish(); yyterminate();

\.data         BEGIN data; dl(".data section"); 
<data>\n       /* just skip */
<data>{WORD}   data_add(YSTRING); dl(".data: %s", yytext);
<data>;        BEGIN INITIAL;

\.arg          BEGIN arg;
<arg>{WORD}    var_add(YSTRING); dl(".arg: %s", yytext); BEGIN eol;

\.export       BEGIN exp;
<exp>{WORD}    exp_add(YSTRING); dl("export: %s", yytext); BEGIN eol;

\.import       BEGIN imp;
<imp>{WORD}    imp_add(YSTRING); dl("import: %s", yytext); BEGIN eol;

\.inherits     BEGIN inh;
<inh>{WORD}    header_set_parent(YSTRING); dl("parent: %s", yytext); BEGIN eol;

\.module       BEGIN mod;
<mod>{WORD}    header_set_name(YSTRING); dl("module %s", yytext); BEGIN eol;

\.str          BEGIN str;
<str>{WORD}    str_begin(YSTRING); dl("string: %s", yytext); BEGIN strsect;
<strsect>\n    /* allowed */
<strsect>\"    BEGIN string;
<strsect>;     BEGIN INITIAL;
<string>"\\"   str_add_char('\\');
<string>\\\"   str_add_char('"');
<string>\\n    str_add_char('\n');
<string>\\t    str_add_char('\t');
<string>\"     BEGIN strsect;
<string>.      str_add_char(*yytext);

\.sub          BEGIN sub;
<sub>{WORD}    ptbl_add(YSTRING); dl("procedure %s", yytext); BEGIN eol;

\.var          BEGIN var;
<var>{WORD}    var_add(YSTRING); dl("variable %s", yytext); BEGIN eol;

<eol>\n        BEGIN INITIAL;

{WORD}:        yytext[--yyleng] = 0; label_add(YSTRING); dl("label: %s", yytext);
