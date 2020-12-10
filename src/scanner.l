%option outfile="src/scanner.c"
%option noyywrap
%option nounput
/* Auto increment variable yylineno every time a '\n' is read. */
%option yylineno

%{
    
#include "types.h"
#include "bison.h"
#include "parser.h" // For the token types from bison.
#include "tables.h"

extern strTable *st;

void yyerror(const char *);  /* prints grammar violation message */
static void comment(void);

%}

%e  1019
%p  2807
%n  371
%k  284
%a  1213
%o  1117

O   [0-7]
D   [0-9]
NZ  [1-9]
L   [a-zA-Z_]
A   [a-zA-Z_0-9]
H   [a-fA-F0-9]
HP  (0[xX])
E   ([Ee][+-]?{D}+)
P   ([Pp][+-]?{D}+)
FS  (f|F|l|L)
IS  (((u|U)(l|L|ll|LL)?)|((l|L|ll|LL)(u|U)?))
CP  (u|U|L)
SP  (u8|u|U|L)
ES  (\\(['"\?\\abfnrtv]|[0-7]{1,3}|x[a-fA-F0-9]+))
WS  [ \t\v\n\f]

%%

"/*"                                { comment(); }
"//".*                              { /* consume //-comment */ }

"break"                             { return(BREAK); }
"char"                              { return(CHAR); }
"do"                                { return(DO); }
"else"                              { return(ELSE); }
"float"                             { return(FLOAT); }
"for"                               { return(FOR); }
"if"                                { return(IF); }
"int"                               { return(INT); }
"return"                            { return(RETURN); }
"while"                             { return(WHILE); }
"void"                              { return(VOID); }

{L}{A}*                             { yylval.sval = strdup(yytext); return IDENTIFIER; }

{HP}{H}+{IS}?				        { yylval.ast = new_node(INT_VAL_NODE, atoi(yytext), INT_TYPE); return I_CONSTANT; }
{NZ}{D}*{IS}?				        { yylval.ast = new_node(INT_VAL_NODE, atoi(yytext), INT_TYPE); return I_CONSTANT; }
"0"{O}*{IS}?				        { yylval.ast = new_node(INT_VAL_NODE, atoi(yytext), INT_TYPE); return I_CONSTANT; }
{CP}?"'"([^'\\\n]|{ES})+"'"		    { yylval.ast = new_node(INT_VAL_NODE, atoi(yytext), INT_TYPE); return I_CONSTANT; }

{D}+{E}{FS}?				        { yylval.ast = new_node(FLOAT_VAL_NODE, 0, FLOAT_TYPE);
                                        set_float_data(yylval.ast, (float) atof(yytext)); return F_CONSTANT; }
{D}*"."{D}+{E}?{FS}?			    { yylval.ast = new_node(FLOAT_VAL_NODE, 0, FLOAT_TYPE);
                                        set_float_data(yylval.ast, (float) atof(yytext)); return F_CONSTANT; }
{D}+"."{E}?{FS}?			        { yylval.ast = new_node(FLOAT_VAL_NODE, 0, FLOAT_TYPE);
                                        set_float_data(yylval.ast, (float) atof(yytext)); return F_CONSTANT; }
{HP}{H}+{P}{FS}?			        { yylval.ast = new_node(FLOAT_VAL_NODE, 0, FLOAT_TYPE);
                                        set_float_data(yylval.ast, (float) atof(yytext)); return F_CONSTANT; }
{HP}{H}*"."{H}+{P}{FS}?			    { yylval.ast = new_node(FLOAT_VAL_NODE, 0, FLOAT_TYPE);
                                        set_float_data(yylval.ast, (float) atof(yytext)); return F_CONSTANT; }
{HP}{H}+"."{P}{FS}?			        { yylval.ast = new_node(FLOAT_VAL_NODE, 0, FLOAT_TYPE);
                                        set_float_data(yylval.ast, (float) atof(yytext)); return F_CONSTANT; }

({SP}?\"([^"\\\n]|{ES})*\"{WS}*)+   { if (strlen(yytext) > 1 )
                                            yylval.ast = new_node(STR_VAL_NODE, strTable_add(st, yytext), CHAR_ARRAY_TYPE);
                                        else
                                            yylval.ast = new_node(CHAR_VAL_NODE, strTable_add(st, yytext), CHAR_TYPE);
                                        return STRING_LITERAL; }


">>="                               { return RIGHT_ASSIGN; }
"<<="                               { return LEFT_ASSIGN; }
"+="                                { return ADD_ASSIGN; }
"-="                                { return SUB_ASSIGN; }
"*="                                { return MUL_ASSIGN; }
"/="                                { return DIV_ASSIGN; }
"%="                                { return MOD_ASSIGN; }
"&="                                { return AND_ASSIGN; }
"^="                                { return XOR_ASSIGN; }
"|="                                { return OR_ASSIGN; }
">>"                                { return RIGHT_OP; }
"<<"                                { return LEFT_OP; }
"&&"                                { return AND_OP; }
"||"                                { return OR_OP; }
"<="                                { return LE_OP; }
">="                                { return GE_OP; }
"=="                                { return EQ_OP; }
"!="                                { return NE_OP; }
";"                                 { return ';'; }
("{"|"<%")                          { return '{'; }
("}"|"%>")                          { return '}'; }
","                                 { return ','; }
"="                                 { return '='; }
"("                                 { return LPAR; }
")"                                 { return RPAR; }
("["|"<:")                          { return '['; }
("]"|":>")                          { return ']'; }
"&"                                 { return '&'; }
"!"                                 { return '!'; }
"~"                                 { return '~'; }
"-"                                 { return '-'; }
"+"                                 { return '+'; }
"*"                                 { return '*'; }
"/"                                 { return '/'; }
"%"                                 { return '%'; }
"<"                                 { return '<'; }
">"                                 { return '>'; }
"^"	                                { return '^'; }
"|"                                 { return '|'; }

{WS}+                               { /* whitespace separates tokens */ }
.                                   { printf("LEXICAL ERROR (%d): Unknown symbol %s\n", yylineno, yytext);
                                        exit(EXIT_FAILURE); }

%%

static void comment(void) {
    int c;
    while ((c = input()) != 0)
        if (c == '*') {
            while ((c = input()) == '*');

            if (c == '/')
                return;

            if (c == 0)
                break;
        }
    yyerror("unterminated comment");
}

// Handle scanner errors (prints grammar violation message)
void yyerror (char const *s) {
    printf("SYNTAX ERROR (%d): %s\n", yylineno, s);
    exit(EXIT_FAILURE);
}
