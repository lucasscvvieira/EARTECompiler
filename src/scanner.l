%option outfile="src/scanner.c"
%option noyywrap
%option nounput

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

%{
    
#include "parser.h" // For the token types from bison.

extern void yyerror(const char *);  /* prints grammar violation message */


static void comment(void);

%}

%%

"/*"                                { comment(); }
"//".*                              { /* consume //-comment */ }

"char"					            { return(CHAR); }
"do"					            { return(DO); }
"else"					            { return(ELSE); }
"float"					            { return(FLOAT); }
"for"					            { return(FOR); }
"if"					            { return(IF); }
"int"					            { return(INT); }
"return"				            { return(RETURN); }
"while"					            { return(WHILE); }
"void"                              { return(VOID); }
"__func__"                          { return FUNC_NAME; }

{L}{A}*					            { return IDENTIFIER; }

{HP}{H}+{IS}?			            { return CONSTANT; }
{NZ}{D}*{IS}?			            { return CONSTANT; }
"0"{O}*{IS}?				        { return CONSTANT; }
{CP}?"'"([^'\\\n]|{ES})+"'"		    { return CONSTANT; }

{D}+{E}{FS}?				        { return CONSTANT; }
{D}*"."{D}+{E}?{FS}?			    { return CONSTANT; }
{D}+"."{E}?{FS}?			        { return CONSTANT; }
{HP}{H}+{P}{FS}?			        { return CONSTANT; }
{HP}{H}*"."{H}+{P}{FS}?			    { return CONSTANT; }
{HP}{H}+"."{P}{FS}?			        { return CONSTANT; }

({SP}?\"([^"\\\n]|{ES})*\"{WS}*)+	{ return STRING_LITERAL; }


">>="					            { return RIGHT_ASSIGN; }
"<<="					            { return LEFT_ASSIGN; }
"+="					            { return ADD_ASSIGN; }
"-="					            { return SUB_ASSIGN; }
"*="					            { return MUL_ASSIGN; }
"/="					            { return DIV_ASSIGN; }
"%="					            { return MOD_ASSIGN; }
"&="					            { return AND_ASSIGN; }
"^="					            { return XOR_ASSIGN; }
"|="					            { return OR_ASSIGN; }
">>"					            { return RIGHT_OP; }
"<<"					            { return LEFT_OP; }
"&&"					            { return AND_OP; }
"||"					            { return OR_OP; }
"<="					            { return LE_OP; }
">="					            { return GE_OP; }
"=="					            { return EQ_OP; }
"!="					            { return NE_OP; }
";"					                { return ';'; }
("{"|"<%")				            { return '{'; }
("}"|"%>")				            { return '}'; }
","					                { return ','; }
":"					                { return ':'; }
"="					                { return '='; }
"("					                { return '('; }
")"					                { return ')'; }
("["|"<:")				            { return '['; }
("]"|":>")				            { return ']'; }
"."					                { return '.'; }
"&"					                { return '&'; }
"!"					                { return '!'; }
"~"					                { return '~'; }
"-"					                { return '-'; }
"+"					                { return '+'; }
"*"					                { return '*'; }
"/"					                { return '/'; }
"%"					                { return '%'; }
"<"					                { return '<'; }
">"					                { return '>'; }
"^"					                { return '^'; }
"|"					                { return '|'; }
"?"					                { return '?'; }

{WS}+					            { /* whitespace separates tokens */ }
.					                { /* discard bad characters */ }

%%

static void comment(void)
{
    int c;

    while ((c = input()) != 0)
        if (c == '*')
        {
            while ((c = input()) == '*')
                ;

            if (c == '/')
                return;

            if (c == 0)
                break;
        }
    yyerror("unterminated comment");
}