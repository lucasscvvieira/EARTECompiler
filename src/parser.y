%output "src/parser.c"          // File name of generated parser.
%defines "src/parser.h"         // Produces a 'parser.h'
%define parse.error verbose		// Give proper messages when a syntax error is found.
%define parse.lac full			// Enable LAC to improve syntax error handling.

%{
#include <stdlib.h>
#include "types.h"
#include "parser.h"

int yylex(void);
int yylex_destroy(void);
extern void yyerror(const char *);  /* prints grammar violation message */

extern void check_var();
extern void new_var();
extern void check_func();
extern void new_func();
extern void new_arg();

Type last_decl_type;
char *last_decl_func = NULL;
%}

%union { char *sval; }

%token	<sval> IDENTIFIER CONSTANT STRING_LITERAL FUNC_NAME
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
	: IDENTIFIER 		{ check_var(); }
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
	| IDENTIFIER { check_func(); } LPAR function_call
	| postfix_expression '.' IDENTIFIER 			{ check_var(); }
	| LPAR type_specifier RPAR '{' initializer_list '}'
	| LPAR type_specifier RPAR '{' initializer_list ',' '}'
	;

function_call
	: RPAR
	| argument_expression_list RPAR
	;

argument_expression_list
	: assignment_expression
	| argument_expression_list ',' assignment_expression
	;

cast_expression
	: postfix_expression
	| LPAR type_specifier RPAR cast_expression
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
	: type_specifier ';'
	| type_specifier init_declarator_list ';'
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
	: CHAR	{ last_decl_type = CHAR_TYPE; }
	| INT	{ last_decl_type = INT_TYPE; }
	| FLOAT	{ last_decl_type = FLOAT_TYPE; }
	| VOID	{ last_decl_type = VOID_TYPE; }
	;

direct_declarator
	: IDENTIFIER 
	| LPAR direct_declarator RPAR
	| direct_declarator '[' ']'
	| direct_declarator '[' '*' ']'
	| direct_declarator '[' assignment_expression ']'
	;

func_declarator
	: IDENTIFIER { new_func(); } LPAR function_declaration
	;

function_declaration
	: RPAR
	| parameter_list RPAR
	| identifier_list RPAR
	;

parameter_list
	: parameter_declaration
	| parameter_list ',' parameter_declaration
	;

parameter_declaration
	: type_specifier direct_parameter_declarator
	| type_specifier
	;

direct_parameter_declarator
	: IDENTIFIER { new_arg(); }
	| LPAR direct_parameter_declarator RPAR
	| direct_parameter_declarator '[' ']'
	| direct_parameter_declarator '[' '*' ']'
	| direct_parameter_declarator '[' assignment_expression ']'
	;

identifier_list
	: IDENTIFIER						{ check_var(); }
	| identifier_list ',' IDENTIFIER	{ check_var(); }
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
	| '{' block_item_list '}'
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
	: function_definition { free(last_decl_func); last_decl_func = NULL; }
	| declaration
	;

function_definition
	: type_specifier func_declarator function_definition_aux
	;

function_definition_aux
	: declaration_list compound_statement
	| compound_statement
	;

declaration_list
	: declaration
	| declaration_list declaration
	;

%%

