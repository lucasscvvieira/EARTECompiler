%output "src/parser.c"          // File name of generated parser.
%defines "src/parser.h"         // Produces a 'parser.h'
%define parse.error verbose // Give proper messages when a syntax error is found.
%define parse.lac full      // Enable LAC to improve syntax error handling.

%{
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "tables.h"
#include "parser.h"

int yylex(void);
void yyerror(char const *s);

void check_var();
void new_var();

extern char *yytext;
extern int yylineno;

StrTable *st;
VarTable *vt;

Type last_decl_type;
%}

%token	IDENTIFIER CONSTANT STRING_LITERAL FUNC_NAME
%token	LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token	AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token	SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token	XOR_ASSIGN OR_ASSIGN
%token	CHAR INT FLOAT VOID

%token	IF ELSE LPAR RPAR WHILE DO FOR RETURN BREAK

%precedence RPAR
%precedence ELSE

%start translation_unit
%%

primary_expression
	: IDENTIFIER	{ check_var(); }
	| CONSTANT
	| string
	| LPAR expression RPAR
	;

string
	: STRING_LITERAL
	| FUNC_NAME
	;

postfix_expression
	: primary_expression
	| postfix_expression '[' expression ']'
	| postfix_expression LPAR RPAR
	| postfix_expression LPAR argument_expression_list RPAR
	| postfix_expression '.' IDENTIFIER { check_var(); }
	| LPAR type_name RPAR '{' initializer_list '}'
	| LPAR type_name RPAR '{' initializer_list ',' '}'
	;

argument_expression_list
	: assignment_expression
	| argument_expression_list ',' assignment_expression
	;

cast_expression
	: postfix_expression
	| LPAR type_name RPAR cast_expression
	;

multiplicative_expression
	: cast_expression
	| multiplicative_expression '*' cast_expression
	| multiplicative_expression '/' cast_expression
	| multiplicative_expression '%' cast_expression
	;

additive_expression
	: multiplicative_expression
	| additive_expression '+' multiplicative_expression
	| additive_expression '-' multiplicative_expression
	;

shift_expression
	: additive_expression
	| shift_expression LEFT_OP additive_expression
	| shift_expression RIGHT_OP additive_expression
	;

relational_expression
	: shift_expression
	| relational_expression '<' shift_expression
	| relational_expression '>' shift_expression
	| relational_expression LE_OP shift_expression
	| relational_expression GE_OP shift_expression
	;

equality_expression
	: relational_expression
	| equality_expression EQ_OP relational_expression
	| equality_expression NE_OP relational_expression
	;

and_expression
	: equality_expression
	| and_expression '&' equality_expression
	;

exclusive_or_expression
	: and_expression
	| exclusive_or_expression '^' and_expression
	;

inclusive_or_expression
	: exclusive_or_expression
	| inclusive_or_expression '|' exclusive_or_expression
	;

logical_and_expression
	: inclusive_or_expression
	| logical_and_expression AND_OP inclusive_or_expression
	;

logical_or_expression
	: logical_and_expression
	| logical_or_expression OR_OP logical_and_expression
	;

assignment_expression
	: logical_or_expression
	| postfix_expression assignment_operator assignment_expression
	;

assignment_operator
	: '='
	| MUL_ASSIGN
	| DIV_ASSIGN
	| MOD_ASSIGN
	| ADD_ASSIGN
	| SUB_ASSIGN
	| LEFT_ASSIGN
	| RIGHT_ASSIGN
	| AND_ASSIGN
	| XOR_ASSIGN
	| OR_ASSIGN
	;

expression
	: assignment_expression
	| expression ',' assignment_expression
	;

declaration
	: declaration_specifiers ';'
	| declaration_specifiers init_declarator_list ';'
	;

declaration_specifiers
	: type_specifier declaration_specifiers
	| type_specifier
	;

init_declarator_list
	: init_declarator
	| init_declarator_list ',' init_declarator
	;

init_declarator
	: direct_declarator '=' initializer
	| direct_declarator
	;

type_specifier
	: CHAR	{ last_decl_type = BOOL_TYPE; }
	| INT	{ last_decl_type = BOOL_TYPE; }
	| FLOAT	{ last_decl_type = BOOL_TYPE; }
	| VOID	{ last_decl_type = BOOL_TYPE; }
	;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list
	| type_specifier
	;

direct_declarator
	: IDENTIFIER { new_var(); }
	| LPAR direct_declarator RPAR
	| direct_declarator '[' ']'
	| direct_declarator '[' '*' ']'
	| direct_declarator '[' assignment_expression ']'
	| direct_declarator LPAR parameter_list RPAR
	| direct_declarator LPAR RPAR
	| direct_declarator LPAR identifier_list RPAR
	;

parameter_list
	: parameter_declaration
	| parameter_list ',' parameter_declaration
	;

parameter_declaration
	: declaration_specifiers direct_declarator
	| declaration_specifiers direct_abstract_declarator
	| declaration_specifiers
	;

identifier_list
	: IDENTIFIER						{ new_var(); }
	| identifier_list ',' IDENTIFIER	{ new_var(); }
	;

type_name
	: specifier_qualifier_list direct_abstract_declarator
	| specifier_qualifier_list
	;

direct_abstract_declarator
	: LPAR direct_abstract_declarator RPAR
	| '[' ']'
	| '[' '*' ']'
	| '[' assignment_expression ']'
	| direct_abstract_declarator '[' ']'
	| direct_abstract_declarator '[' '*' ']'
	| direct_abstract_declarator '[' assignment_expression ']'
	| LPAR RPAR
	| LPAR parameter_list RPAR
	| direct_abstract_declarator LPAR RPAR
	| direct_abstract_declarator LPAR parameter_list RPAR
	;

initializer
	: '{' initializer_list '}'
	| '{' initializer_list ',' '}'
	| assignment_expression
	;

initializer_list
	: designation initializer
	| initializer
	| initializer_list ',' designation initializer
	| initializer_list ',' initializer
	;

designation
	: designator_list '='
	;

designator_list
	: designator
	| designator_list designator
	;

designator
	: '[' logical_or_expression ']'
	| '.' IDENTIFIER				{ check_var(); }
	;

statement
	: compound_statement
	| expression_statement
	| selection_statement
	| iteration_statement
	| jump_statement
	;

compound_statement
	: '{' '}'
	| '{'  block_item_list '}'
	;

block_item_list
	: block_item
	| block_item_list block_item
	;

block_item
	: declaration
	| statement
	;

expression_statement
	: ';'
	| expression ';'
	;

selection_statement
	: IF LPAR expression RPAR statement ELSE statement
	| IF LPAR expression RPAR statement
	;

iteration_statement
	: WHILE LPAR expression RPAR statement
	| DO statement WHILE LPAR expression RPAR ';'
	| FOR LPAR expression_statement expression_statement RPAR statement
	| FOR LPAR expression_statement expression_statement expression RPAR statement
	| FOR LPAR declaration expression_statement RPAR statement
	| FOR LPAR declaration expression_statement expression RPAR statement
	;

jump_statement
	: RETURN ';'
	| RETURN expression ';'
	| BREAK ';'
	;

translation_unit
	: external_declaration
	| translation_unit external_declaration
	;

external_declaration
	: function_definition
	| declaration
	;

function_definition
	: declaration_specifiers direct_declarator declaration_list compound_statement
	| declaration_specifiers direct_declarator compound_statement
	;

declaration_list
	: declaration
	| declaration_list declaration
	;

%%

void check_var() {
    int idx = lookup_var(vt, yytext);
    if (idx == -1) {
        printf("SEMANTIC ERROR (%d): variable '%s' was not declared.\n",
                yylineno, yytext);
        exit(EXIT_FAILURE);
    }
}

void new_var() {
    int idx = lookup_var(vt, yytext);
    if (idx != -1) {
        printf("SEMANTIC ERROR (%d): variable '%s' already declared at line %d.\n",
                yylineno, yytext, get_line(vt, idx));
        exit(EXIT_FAILURE);
    }
    add_var(vt, yytext, yylineno, last_decl_type);
}

void yyerror (char const *s) {
    printf("SYNTAX ERROR (%d): %s\n", yylineno, s);
    exit(EXIT_FAILURE);
}
