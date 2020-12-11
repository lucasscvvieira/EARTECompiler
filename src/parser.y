%output "src/parser.c"          // File name of generated parser.
%defines "src/parser.h"         // Produces a 'parser.h'
%define parse.error verbose		// Give proper messages when a syntax error is found.
%define parse.lac full			// Enable LAC to improve syntax error handling.

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "utils.h"
#include "bison.h"
#include "types.h"
#include "tables.h"
#include "parser.h"

int yylex(void);
int yylex_destroy(void);
extern void yyerror(const char *);  /* prints grammar violation message */
extern strTable *st;
extern AST *root;

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

%start start_program

%%

primary_expression
	: IDENTIFIER 			{ $$.ast = check_var(); }
	| I_CONSTANT			{ $$.ast = $1.ast; }
	| F_CONSTANT			{ $$.ast = $1.ast; }
	| STRING_LITERAL		{ $$.ast = $1.ast; }
	| LPAR expression RPAR	{ $$.ast = $2.ast; }
	;

postfix_expression
	: primary_expression { $$.ast = $1.ast; }
	| primary_expression '[' expression { set_node_type($1.ast, back_to_non_array_type(get_node_type($1.ast))); }
		']' { $$.ast = $1.ast; }

	| IDENTIFIER { last_called_func = strdup($1.sval); $1.ast = check_func(); }
		LPAR { arg_num = 0; }
		function_call { $$.ast = new_subtree(BLOCK_NODE, get_node_type($1.ast), 2, $1.ast, $5.ast); }

	| LPAR type_specifier RPAR '{' initializer_list '}' { $$.ast = $5.ast; }
	| LPAR type_specifier RPAR '{' initializer_list ',' '}' { $$.ast = $5.ast; }
	;

function_call
	: RPAR							{ $$.ast = new_subtree(VAR_LIST_NODE, NO_TYPE, 0); }
	| argument_expression_list RPAR { $$.ast = $1.ast; }
	;

argument_expression_list
	: assignment_expression									{ $$.ast = new_subtree(BLOCK_NODE, NO_TYPE, 1, check_arg()); arg_num++; } 
	| argument_expression_list ',' assignment_expression	{ add_child($1.ast, check_arg()); $$ = $1; arg_num++; }
	;

cast_expression
	: postfix_expression						{ $$.ast = $1.ast; }
	| LPAR type_specifier RPAR cast_expression	{ $2.ast = cast($4.ast); $$ = $2; }
	;

multiplicative_expression
	: cast_expression								{ $$.ast = $1.ast; }
	| multiplicative_expression '*' cast_expression	{ $$.ast = unify_bin_node($1.ast, $3.ast, MUL_NODE, "*", unify_other_arith); }
	| multiplicative_expression '/' cast_expression	{ $$.ast = unify_bin_node($1.ast, $3.ast, DIV_NODE, "/", unify_other_arith); }
	| multiplicative_expression '%' cast_expression	{ $$.ast = unify_bin_node($1.ast, $3.ast, MOD_NODE, "%", unify_logic); }
	;

additive_expression
	: multiplicative_expression							{ $$.ast = $1.ast; }
	| additive_expression '+' multiplicative_expression	{ $$.ast = unify_bin_node($1.ast, $3.ast, PLUS_NODE, "+", unify_plus); }
	| additive_expression '-' multiplicative_expression { $$.ast = unify_bin_node($1.ast, $3.ast, MINUS_NODE, "-", unify_other_arith); }
	;

shift_expression
	: additive_expression							{ $$.ast = $1.ast; }
	| shift_expression LEFT_OP additive_expression	{ $$.ast = unify_bin_node($1.ast, $3.ast, LEFT_NODE, "<<", unify_logic); }
	| shift_expression RIGHT_OP additive_expression	{ $$.ast = unify_bin_node($1.ast, $3.ast, RIGHT_NODE, ">>", unify_logic); }
	;

relational_expression
	: shift_expression								{ $$.ast = $1.ast; }
	| relational_expression '<' shift_expression	{ $$.ast = unify_bin_node($1.ast, $3.ast, LT_NODE, "<", unify_comp); }
	| relational_expression '>' shift_expression	{ $$.ast = unify_bin_node($1.ast, $3.ast, GT_NODE, ">", unify_comp); }
	| relational_expression LE_OP shift_expression	{ $$.ast = unify_bin_node($1.ast, $3.ast, LTE_NODE, "<=", unify_comp); }
	| relational_expression GE_OP shift_expression	{ $$.ast = unify_bin_node($1.ast, $3.ast, GTE_NODE, ">=", unify_comp); }
	;

equality_expression
	: relational_expression								{ $$.ast = $1.ast; }
	| equality_expression EQ_OP relational_expression 	{ $$.ast = unify_bin_node($1.ast, $3.ast, EQ_NODE, "==", unify_comp); }
	| equality_expression NE_OP relational_expression 	{ $$.ast = unify_bin_node($1.ast, $3.ast, NE_NODE, "!=", unify_comp); }
	;

and_expression
	: equality_expression						{ $$.ast = $1.ast; }
	| and_expression '&' equality_expression	{ $$.ast = unify_bin_node($1.ast, $3.ast, AND_NODE, "&", unify_logic); }
	;

exclusive_or_expression
	: and_expression								{ $$.ast = $1.ast; }
	| exclusive_or_expression '^' and_expression	{ $$.ast = unify_bin_node($1.ast, $3.ast, XOR_NODE, "^", unify_logic); }
	;

inclusive_or_expression
	: exclusive_or_expression								{ $$.ast = $1.ast; }
	| inclusive_or_expression '|' exclusive_or_expression	{ $$.ast = unify_bin_node($1.ast, $3.ast, OR_NODE, "|", unify_logic); }
	;

logical_and_expression
	: inclusive_or_expression								{ $$.ast = $1.ast; }
	| logical_and_expression AND_OP inclusive_or_expression	{ $$.ast = unify_bin_node($1.ast, $3.ast, AND_NODE, "&&", unify_logic); }
	;

logical_or_expression
	: logical_and_expression								{ $$.ast = $1.ast; }
	| logical_or_expression OR_OP logical_and_expression	{ $$.ast = unify_bin_node($1.ast, $3.ast, OR_NODE, "||", unify_logic); }
	;

assignment_expression
	: logical_or_expression											{ $$.ast = $1.ast; }
	| postfix_expression assignment_operator assignment_expression	{ $$.ast = check_assign($1.ast, $2.sval, $3.ast); }
	;

assignment_operator
	: '='				{ strcpy($$.sval, "="); }
	| MUL_ASSIGN		{ strcpy($$.sval, "*="); }
	| DIV_ASSIGN		{ strcpy($$.sval, "/="); }
	| MOD_ASSIGN		{ strcpy($$.sval, "%="); }
	| ADD_ASSIGN		{ strcpy($$.sval, "+="); }
	| SUB_ASSIGN		{ strcpy($$.sval, "-="); }
	| LEFT_ASSIGN		{ strcpy($$.sval, "<<="); }
	| RIGHT_ASSIGN		{ strcpy($$.sval, ">>="); }
	| AND_ASSIGN		{ strcpy($$.sval, "&="); }
	| XOR_ASSIGN		{ strcpy($$.sval, "^="); }
	| OR_ASSIGN			{ strcpy($$.sval, "|="); }
	;

expression
	: assignment_expression					{ $$.ast = new_subtree(BLOCK_NODE, get_node_type($1.ast), 1, $1.ast); }
	| expression ',' assignment_expression	{ add_child($1.ast, $3.ast); $$.ast = $1.ast; }
	;

init_declarator_list
	: init_declarator								{ $$.ast = new_subtree(BLOCK_NODE, NO_TYPE, 1, $1.ast); }
	| init_declarator_list ',' init_declarator		{ add_child($1.ast, $3.ast); $$.ast = $1.ast; }
	;

init_declarator
	: direct_declarator { $1.ast = new_var();
						  last_decl_type = back_to_non_array_type(last_decl_type);
						  $$.ast = $1.ast; }
	| direct_declarator { $1.ast = new_var();
						  last_decl_type = back_to_non_array_type(last_decl_type);
						} '=' initializer { $$.ast = new_subtree(ASSIGN_NODE, NO_TYPE, 2, $1.ast, $4.ast); }
	;

type_specifier
	: CHAR	{ last_decl_type = CHAR_TYPE; }
	| INT	{ last_decl_type = INT_TYPE; }
	| FLOAT	{ last_decl_type = FLOAT_TYPE; }
	| VOID	{ last_decl_type = VOID_TYPE; }
	;

direct_declarator
	: IDENTIFIER					{ $$.sval = strdup($1.sval); $$.ast = new_subtree(BLOCK_NODE, NO_TYPE, 0); }
	| LPAR direct_declarator RPAR	{ $$.sval = strdup($2.sval); $$.ast = $2.ast;}

	| direct_declarator '[' { last_decl_type = get_array_type(last_decl_type); }
		direct_declarator_array		{ $$.sval = strdup($1.sval); add_child($1.ast, $4.ast); $$.ast = $1.ast; }

	| direct_declarator { $1.ast = new_func(); } LPAR function_declaration	{ $$.ast = $1.ast; }
	;

direct_declarator_array
	: ']'						{ $$.ast = new_subtree(VAR_LIST_NODE, NO_TYPE, 0); }
	| '*' ']'					{ $$.ast = new_subtree(VAR_LIST_NODE, NO_TYPE, 0); }
	| assignment_expression ']'	{ $$.ast = $1.ast; }
	;

function_declaration
	: RPAR					{ $$.ast = new_subtree(VAR_LIST_NODE, NO_TYPE, 0); }
	| parameter_list RPAR	{ $$.ast = $1.ast; }
	;

parameter_list
	: type_specifier direct_parameter_declarator					{ new_arg(); $$.ast = new_subtree(BLOCK_NODE, NO_TYPE, 1, $2.ast); }
	| parameter_list ',' type_specifier direct_parameter_declarator	{ new_arg(); add_child($1.ast, $4.ast); $$.ast = $1.ast; }
	;

direct_parameter_declarator
	: IDENTIFIER							{ $$.sval = strdup($1.sval); $$.ast = new_subtree(BLOCK_NODE, NO_TYPE, 1, $1.ast); }
	| LPAR direct_parameter_declarator RPAR { $$.sval = strdup($2.sval); $$.ast = $2.ast; }

	| direct_parameter_declarator '[' { last_decl_type = get_array_type(last_decl_type); }
		direct_declarator_array				{ $$.sval = strdup($1.sval); add_child($1.ast, $4.ast); $$.ast = $1.ast; }
	;

initializer
	: '{' initializer_list '}'	{ $$.ast = $2.ast; }
	| assignment_expression		{ $$.ast = $1.ast; }
	;

initializer_list
	: assignment_expression							{ $$.ast = new_subtree(BLOCK_NODE, NO_TYPE, 1, $1.ast); }
	| initializer_list ',' assignment_expression	{ add_child($1.ast, $3.ast); $$.ast = $1.ast; }
	;


statement
	: compound_statement	{ $$.ast = $1.ast; }
	| expression_statement	{ $$.ast = $1.ast; }
	| selection_statement	{ $$.ast = $1.ast; }
	| iteration_statement	{ $$.ast = $1.ast; }
	| jump_statement		{ $$.ast = $1.ast; }
	;

compound_statement
	: '{' '}'					{ $$.ast = new_subtree(BLOCK_NODE, NO_TYPE, 0); }
	| '{' block_item_list '}'	{ $$.ast = $2.ast; }
	;

block_item_list
	: block_item					{ $$.ast = new_subtree(BLOCK_NODE, NO_TYPE, 1, $1.ast); }
	| block_item_list block_item	{ add_child($1.ast, $2.ast); $$.ast = $1.ast; }
	;

block_item
	: type_specifier init_declarator_list ';'	{ $$.ast = $2.ast; }
	| statement									{ $$.ast = $1.ast; }
	;

expression_statement
	: ';'				{ $$.ast = new_subtree(VAR_LIST_NODE, NO_TYPE, 0); }
	| expression ';'	{ $$.ast = $1.ast; }
	;

selection_statement
	: IF LPAR expression RPAR statement ELSE statement	{ $$.ast = check_if_then_else($3.ast, $5.ast, $7.ast);}
	| IF LPAR expression RPAR statement					{ $$.ast = check_if_then($3.ast, $5.ast); }
	;

iteration_statement
	: WHILE LPAR expression RPAR statement																
		{ $$.ast = check_while($3.ast, $5.ast); }

	| DO statement WHILE LPAR expression RPAR ';'														
		{ $$.ast = check_do($5.ast, $2.ast); }

	| FOR LPAR expression_statement expression_statement RPAR statement									
		{ $$.ast = check_for_AC($3.ast, $4.ast, $6.ast); }

	| FOR LPAR expression_statement expression_statement expression RPAR statement						
		{ $$.ast = check_for_ACI($3.ast, $4.ast, $5.ast, $7.ast); }

	| FOR LPAR type_specifier init_declarator_list ';' expression_statement RPAR statement				
		{ $$.ast = check_for_DC($4.ast, $6.ast, $8.ast); }

	| FOR LPAR type_specifier init_declarator_list ';' expression_statement expression RPAR statement	
		{ $$.ast = check_for_DCI($4.ast, $6.ast, $7.ast, $9.ast); }
	;

jump_statement
	: RETURN ';' 			{ check_return(VOID_TYPE); $$.ast = new_subtree(RETURN, NO_TYPE, 0); }
	| RETURN expression ';' { check_return(get_node_type($2.ast));
								last_return_type = get_node_type($2.ast);
								$$.ast = new_subtree(RETURN_NODE, NO_TYPE, 1, $2.ast);}
	| BREAK ';'				{ $$.ast = new_subtree(BREAK_NODE, NO_TYPE, 0); }
	;

start_program
	: translation_unit { root = new_subtree(PROGRAM_NODE, NO_TYPE, 1, $1.ast); }

translation_unit
	: external_declaration 					{ $$.ast = new_subtree(BLOCK_NODE, NO_TYPE, 1, $1.ast); }
	| translation_unit external_declaration	{ add_child($1.ast, $2.ast); $$.ast = $1.ast; }
	;

external_declaration
	: function_definition	{ free(last_decl_func); last_decl_func = NULL; $$.ast = $1.ast; }
	| type_specifier init_declarator_list ';'			{ $$.ast = $2.ast; }
	;

function_definition
	: type_specifier direct_declarator { last_return_type = VOID_TYPE; }
		function_definition_aux { check_return(last_return_type); $$.ast = new_subtree(BLOCK_NODE, NO_TYPE, 2, $2.ast, $4.ast);}
	;

function_definition_aux
	: declaration_list compound_statement	{ $$.ast = new_subtree(BLOCK_NODE, NO_TYPE, 2, $1.ast, $2.ast); }
	| compound_statement					{ $$.ast = $1.ast;}
	;

declaration_list
	: type_specifier init_declarator_list ';'					{ $$.ast = $1.ast; }
	| declaration_list type_specifier init_declarator_list ';'	{ add_child($1.ast, $3.ast); $$.ast = $1.ast; }
	;

%%
