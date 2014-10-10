%top{
#include "print.h"
#include "sect.h"
#include "cstring.h"
#define YSTRING string_new(yytext, yyleng + 1)
#define dl(str) debug_line(str ": %s", yytext)
static void emit_instruction(int len, char opcode, const char *argstring);
static void end_instruction(void);
static const char *args;
uint32_t instr_start;
}

SYM    [a-zA-Z0-9_]
SYM1   [a-zA-Z_]
WORD   {SYM1}{SYM}*
NUM    [0-9]+
CONST  {NUM}|'.'|'\\.'
VER    {NUM}\.{NUM}
EXTPR  {WORD}:{WORD}

%option yylineno nounput noyywrap
%x instruction eol ext sub arg var exp imp impver data mod modver inh
%x str strsect string

%%

\n             /* nothing special */
<INITIAL><<EOF>> sect_finish(); yyterminate();

\.data         BEGIN data;
<data>{WORD}   data_add(YSTRING); dl("data");
<data>\n       BEGIN INITIAL;

\.arg          BEGIN arg;
<arg>{WORD}    arg_add(YSTRING); dl("arg");
<arg>\n        BEGIN INITIAL;

\.export       BEGIN exp;
<exp>{WORD}    exp_add(YSTRING); dl("export"); BEGIN eol;

\.extern       BEGIN ext;
<ext>{EXTPR}   imp_add(YSTRING); dl("extern"); BEGIN eol;

\.import       BEGIN imp;
<imp>{WORD}    module_add(YSTRING); dl("import"); BEGIN impver;
<impver>{VER}  module_set_version(yytext); dl("version"); BEGIN eol;
<impver>\n     /* no version */ BEGIN INITIAL;

\.inherits     BEGIN inh;
<inh>{WORD}    header_set_parent(YSTRING); dl("parent"); BEGIN eol;

\.module       BEGIN mod;
<mod>{WORD}    header_set_name(YSTRING); dl("module"); BEGIN modver;
<modver>{VER}  header_set_version(yytext); dl("version"); BEGIN eol;
<modver>\n     /* no version */ BEGIN INITIAL;

\.str          BEGIN str;
<str>{WORD}    str_begin(YSTRING); dl("string"); BEGIN strsect;
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
<sub>{WORD}    ptbl_add(YSTRING); dl("procedure"); BEGIN eol;

\.var          BEGIN var;
<var>{WORD}    var_add(YSTRING); dl("variable");
<var>\n        BEGIN INITIAL;

<eol>\n        BEGIN INITIAL;

{WORD}:        yytext[--yyleng] = 0; label_add(YSTRING); dl("label");
