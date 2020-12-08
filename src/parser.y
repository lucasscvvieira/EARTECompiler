%output "src/parser.c"          // File name of generated parser.
%defines "src/parser.h"         // Produces a 'parser.h'
%define parse.error verbose		// Give proper messages when a syntax error is found.
%define parse.lac full			// Enable LAC to improve syntax error handling.

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "bison.h"
#include "types.h"
#include "tables.h"
#include "parser.h"

int yylex(void);
int yylex_destroy(void);
extern void yyerror(const char *);  /* prints grammar violation message */
extern strTable *st;

Type last_decl_type;
Type last_return_type;
char *last_decl_func = NULL;
char *last_called_func = NULL;
int arg_num = 0;

%}

%define api.value.type {bison_type}

%token	IDENTIFIER STRING_LITERAL
%token  I_CONSTANT F_CONSTANT
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
	: IDENTIFIER 			{ $$.type = check_var(); }
	| I_CONSTANT			{ $$.type = INT_TYPE; }
	| F_CONSTANT			{ $$.type = FLOAT_TYPE; }

	| STRING_LITERAL		{ if (strlen(strTable_get(st, strTable_length(st) - 1)) > 1 )
								$$.type = CHAR_ARRAY_TYPE;
							else
							  	$$.type = CHAR_TYPE;
							}

	| LPAR expression RPAR	{ $$.type = $2.type; }
	;

postfix_expression
	: primary_expression { $$.type = $1.type; }
	| postfix_expression '[' expression { $1.type = back_to_non_array_type($1.type); }
		']' { $$.type = $1.type; }

	| IDENTIFIER { last_called_func = strdup($1.sval); $1.type = check_func(); }
		LPAR { arg_num = 0; }
		function_call { $$.type = $1.type; }

	| LPAR type_specifier RPAR '{' initializer_list '}' { $$.type = $2.type; }
	| LPAR type_specifier RPAR '{' initializer_list ',' '}' { $$.type = $2.type; }
	;

function_call
	: RPAR
	| argument_expression_list RPAR
	;

argument_expression_list
	: assignment_expression									{ check_arg(); arg_num++; } 
	| argument_expression_list ',' assignment_expression	{ check_arg(); arg_num++; }
	;

cast_expression
	: postfix_expression						{ $$.type = $1.type; }
	| LPAR type_specifier RPAR cast_expression	{ $$.type = $2.type; }
	;

multiplicative_expression
	: cast_expression								{ $$.type = $1.type; }
	| multiplicative_expression '*' cast_expression	{ $$.type = unify_bin_op($1.type, $3.type, "*", unify_other_arith); }
	| multiplicative_expression '/' cast_expression	{ $$.type = unify_bin_op($1.type, $3.type, "/", unify_other_arith); }
	| multiplicative_expression '%' cast_expression	{ $$.type = unify_bin_op($1.type, $3.type, "%", unify_other_arith); }
	;

additive_expression
	: multiplicative_expression							{ $$.type = $1.type;}
	| additive_expression '+' multiplicative_expression	{ $$.type = unify_bin_op($1.type, $3.type, "+", unify_plus); }
	| additive_expression '-' multiplicative_expression { $$.type = unify_bin_op($1.type, $3.type, "-", unify_other_arith); }
	;

shift_expression
	: additive_expression							{ $$.type = $1.type; }
	| shift_expression LEFT_OP additive_expression	{ $$.type = $1.type; }
	| shift_expression RIGHT_OP additive_expression	{ $$.type = $1.type; }
	;

relational_expression
	: shift_expression								{ $$.type = $1.type; }
	| relational_expression '<' shift_expression	{ $$.type = unify_bin_op($1.type, $3.type, "<", unify_comp); }
	| relational_expression '>' shift_expression	{ $$.type = unify_bin_op($1.type, $3.type, ">", unify_comp); }
	| relational_expression LE_OP shift_expression	{ $$.type = unify_bin_op($1.type, $3.type, "<=", unify_comp); }
	| relational_expression GE_OP shift_expression	{ $$.type = unify_bin_op($1.type, $3.type, ">=", unify_comp); }
	;

equality_expression
	: relational_expression								{ $$.type = $1.type; }
	| equality_expression EQ_OP relational_expression 	{ $$.type = unify_bin_op($1.type, $3.type, "==", unify_comp); }
	| equality_expression NE_OP relational_expression 	{ $$.type = unify_bin_op($1.type, $3.type, "!=", unify_comp); }
	;

and_expression
	: equality_expression						{ $$.type = $1.type; }
	| and_expression '&' equality_expression	{ $$.type = unify_bin_op($1.type, $3.type, "&", unify_logic); }
	;

exclusive_or_expression
	: and_expression								{ $$.type = $1.type; }
	| exclusive_or_expression '^' and_expression	{ $$.type = unify_bin_op($1.type, $3.type, "^", unify_logic); }
	;

inclusive_or_expression
	: exclusive_or_expression								{ $$.type = $1.type; }
	| inclusive_or_expression '|' exclusive_or_expression	{ $$.type = unify_bin_op($1.type, $3.type, "|", unify_logic); }
	;

logical_and_expression
	: inclusive_or_expression								{ $$.type = $1.type; }
	| logical_and_expression AND_OP inclusive_or_expression	{ $$.type = unify_bin_op($1.type, $3.type, "&&", unify_logic); }
	;

logical_or_expression
	: logical_and_expression								{ $$.type = $1.type; }
	| logical_or_expression OR_OP logical_and_expression	{ $$.type = unify_bin_op($1.type, $3.type, "||", unify_logic); }
	;

assignment_expression
	: logical_or_expression											{ $$.type = $1.type; }
	| postfix_expression assignment_operator assignment_expression	{ check_assign($1.type, $3.type); $$.type = $1.type; }
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
	: assignment_expression					{ $$.type = $1.type; }
	| expression ',' assignment_expression	{ $$.type = $3.type; }
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
	: direct_declarator { new_var();
						  last_decl_type = back_to_non_array_type(last_decl_type);
						} init_direct_declarator
	;

init_direct_declarator
	: %empty
	| '=' initializer
	;

type_specifier
	: CHAR	{ last_decl_type = CHAR_TYPE; $$.type = CHAR_TYPE; }
	| INT	{ last_decl_type = INT_TYPE; $$.type = INT_TYPE; }
	| FLOAT	{ last_decl_type = FLOAT_TYPE; $$.type = FLOAT_TYPE; }
	| VOID	{ last_decl_type = VOID_TYPE; $$.type = VOID_TYPE; }
	;

direct_declarator
	: IDENTIFIER					{ $$.sval = strdup($1.sval); }
	| LPAR direct_declarator RPAR	{ $$.sval = strdup($2.sval); }

	| direct_declarator '[' { last_decl_type = get_array_type(last_decl_type); }
		direct_declarator_array		{ $$.sval = strdup($1.sval); }

	| direct_declarator { new_func(); } LPAR function_declaration
	;

direct_declarator_array
	: ']'
	| '*' ']'
	| assignment_expression ']'
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
	: type_specifier direct_parameter_declarator { new_arg(); }
	| type_specifier
	;

direct_parameter_declarator
	: IDENTIFIER							{ $$.sval = strdup($1.sval); }
	| LPAR direct_parameter_declarator RPAR { $$.sval = strdup($2.sval); }

	| direct_parameter_declarator '[' { last_decl_type = get_array_type(last_decl_type); }
		direct_parameter_declarator_array	{ $$.sval = strdup($1.sval); }
	;

direct_parameter_declarator_array
	: ']'
	| '*' ']'
	| assignment_expression ']'
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
	: '[' logical_or_expression ']'
	| designator_list '[' logical_or_expression ']'
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
	: IF LPAR expression RPAR statement ELSE statement	{ check_bool_expr("if", $3.type); }
	| IF LPAR expression RPAR statement					{ check_bool_expr("if", $3.type); }
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
	: RETURN ';' { check_return(VOID_TYPE); }
	| RETURN expression ';' { check_return($2.type); last_return_type = $2.type; }
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
	: type_specifier direct_declarator { last_return_type = VOID_TYPE; } function_definition_aux { check_return(last_return_type); }
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
