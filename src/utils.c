#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "bison.h"
#include "types.h"
#include "tables.h"
#include "parser.h"

// main
extern varTable *vt;
extern funcTable *ft;

// scanner
extern int yylineno;

// parser
extern Type last_decl_type;
extern char *last_decl_func;
extern char *last_called_func;
extern int arg_num;

AST *check_func()
{
	int idx = funcTable_lookup(ft, yylval.sval);
	if (idx == -1) {
		printf("SEMANTIC ERROR (%d): function '%s' was not declared.\n",
		       yylineno, yylval.sval);
		exit(EXIT_FAILURE);
	}
	//printf("check_func %s\n", get_text(funcTable_get_type(ft, idx)));
	return new_node(FUNC_CALL_NODE, idx, funcTable_get_type(ft, idx));
}

AST *new_func()
{
	int idx = funcTable_lookup(ft, yylval.sval);

	if (idx != -1) {
		printf
		    ("SEMANTIC ERROR (%d): function '%s' already declared at line %d.\n",
		     yylineno, yylval.sval, funcTable_get_line(ft, idx));
		exit(EXIT_FAILURE);
	}
	// Updates the auxiliary variable with the defined function name
	last_decl_func = strdup(yylval.sval);
	idx = funcTable_add(ft, yylval.sval, yylineno, last_decl_type);
	return new_node(FUNC_DECL_NODE, idx, last_decl_type);
}

void check_return(Type type)
{
	int idx = funcTable_lookup(ft, last_decl_func);
	Type func_type = funcTable_get_type(ft, idx);
	if (type != func_type) {
		printf
		    ("SEMANTIC ERROR (%d): return of type '%s' is different of declared type '%s' of function '%s'.\n",
		     yylineno, get_text(type), get_text(func_type),
		     last_decl_func);
		exit(EXIT_FAILURE);
	}
}

void new_arg()
{
	if (last_decl_func == NULL) {
		printf
		    ("SEMANTIC ERROR (%d): argument '%s' for undeclared function.\n",
		     yylineno, yylval.sval);
		exit(EXIT_FAILURE);
	}
	varTable *args = funcTable_get_args(ft,
					    funcTable_lookup(ft,
							     last_decl_func));

	int idx = varTable_lookup(args, yylval.sval);
	if (idx != -1) {
		printf
		    ("SEMANTIC ERROR (%d): argument '%s' already declared at line %d.\n",
		     yylineno, yylval.sval, varTable_get_line(args, idx));
		exit(EXIT_FAILURE);
	}

	idx = varTable_add(args, yylval.sval, yylineno, last_decl_type);
}

AST *check_arg()
{
	int idx = -1;
	varTable *aux = NULL;

	varTable *args =
	    funcTable_get_args(ft, funcTable_lookup(ft, last_called_func));
	if (varTable_length(args) <= arg_num) {
		printf
		    ("SEMANTIC ERROR (%d): '%s' extra argument for '%s' function .\n",
		     yylineno, yylval.sval, last_called_func);
		exit(EXIT_FAILURE);
	}
	//
	if (last_decl_func != NULL) {
		// Verify if variable is a function argument
		aux =
		    funcTable_get_args(ft,
				       funcTable_lookup(ft, last_decl_func));
		idx = varTable_lookup(aux, yylval.sval);

		// If not found, verify if variable is a function variable
		if (idx == -1) {
			aux =
			    funcTable_get_vt(ft,
					     funcTable_lookup(ft,
							      last_decl_func));
			idx = varTable_lookup(aux, yylval.sval);
		}
	}
	// If not found, verify if variable is a global variable
	if (idx == -1) {
		aux = vt;
		idx = varTable_lookup(aux, yylval.sval);
	}

	if (idx == -1) {
		printf("SEMANTIC ERROR (%d): variable '%s' was not declared.\n",
		       yylineno, yylval.sval);
		exit(EXIT_FAILURE);
	}
	Type arg_type = varTable_get_type(aux, idx);
	//

	Type func_arg_type = varTable_get_type(args, arg_num);
	if (arg_type != func_arg_type) {
		printf
		    ("SEMANTIC ERROR (%d): argument '%s' of type '%s' expecting type '%s' .\n",
		     yylineno, yylval.sval, get_text(arg_type),
		     get_text(func_arg_type));
		exit(EXIT_FAILURE);
	}

	AST *ret = new_node(ARG_DECL_NODE, idx, arg_type);
	set_name(ret, varTable_get_name(aux, idx));
	return ret;
}

AST *check_var()
{
	int type = 0;
	int idx = -1;
	varTable *aux = NULL;

	if (last_decl_func != NULL) {
		// Verify if variable is a function argument
		aux =
		    funcTable_get_args(ft,
				       funcTable_lookup(ft, last_decl_func));
		idx = varTable_lookup(aux, yylval.sval);
		type = ARG_USE_NODE;

		// If not found, verify if variable is a function variable
		if (idx == -1) {
			aux =
			    funcTable_get_vt(ft,
					     funcTable_lookup(ft,
							      last_decl_func));
			idx = varTable_lookup(aux, yylval.sval);
			type = VAR_USE_NODE;
		}
	}
	// If not found, verify if variable is a global variable
	if (idx == -1) {
		aux = vt;
		idx = varTable_lookup(aux, yylval.sval);
		type = GLOBAL_VAR_USE_NODE;
	}

	if (idx == -1) {
		printf("SEMANTIC ERROR (%d): variable '%s' was not declared.\n",
		       yylineno, yylval.sval);
		exit(EXIT_FAILURE);
	}

	AST *ret = new_node(type, idx, varTable_get_type(aux, idx));
	set_name(ret, varTable_get_name(aux, idx));
	return ret;
}

AST *new_var()
{
	int idx = -1;
	varTable *aux = NULL;

	if (last_decl_func != NULL) {
		// Verify if variable is a function argument
		aux =
		    funcTable_get_args(ft,
				       funcTable_lookup(ft, last_decl_func));
		idx = varTable_lookup(aux, yylval.sval);

		// If not found, verify if variable is a function variable
		if (idx == -1) {
			aux =
			    funcTable_get_vt(ft,
					     funcTable_lookup(ft,
							      last_decl_func));
			idx = varTable_lookup(aux, yylval.sval);
		}
	}
	// If not found, verify if variable is a global variable
	if (idx == -1) {
		aux = vt;
		idx = varTable_lookup(aux, yylval.sval);
	}

	if (idx != -1) {
		printf
		    ("SEMANTIC ERROR (%d): variable '%s' already declared at line %d.\n",
		     yylineno, yylval.sval, varTable_get_line(aux, idx));
		exit(EXIT_FAILURE);
	}

	if (last_decl_type == VOID_TYPE || last_decl_type == NO_TYPE) {
		printf
		    ("SEMANTIC ERROR (%d): variable '%s' of type '%s'.\n",
		     yylineno, yylval.sval, get_text(last_decl_type));
		exit(EXIT_FAILURE);
	}
	AST *ret;
	if (last_decl_func != NULL) {
		varTable_add(funcTable_get_vt(ft,
					      funcTable_lookup(ft,
							       last_decl_func)),
			     yylval.sval, yylineno, last_decl_type);
		ret = new_node(VAR_DECL_NODE, idx, last_decl_type);
	} else {
		idx = varTable_add(vt, yylval.sval, yylineno, last_decl_type);
		ret = new_node(GLOBAL_VAR_DECL_NODE, idx, last_decl_type);
	}
	//printf("new_var %s %s\n", yylval.sval, kind2str(get_kind(ret)));
	set_name(ret, yylval.sval);
	return ret;
}

// ----------------------------------------------------------------------------

// Type checking and inference.

void type_error(const char *op, Type t1, Type t2)
{
	printf
	    ("SEMANTIC ERROR (%d): incompatible types for operator '%s', LHS is '%s' and RHS is '%s'.\n",
	     yylineno, op, get_text(t1), get_text(t2));
	exit(EXIT_FAILURE);
}

Type get_array_type(Type type)
{
	switch (type) {
	case INT_TYPE:
		return INT_ARRAY_TYPE;
	case FLOAT_TYPE:
		return FLOAT_ARRAY_TYPE;
	case CHAR_TYPE:
		return CHAR_ARRAY_TYPE;

	case INT_ARRAY_TYPE:
		return type;
	case FLOAT_ARRAY_TYPE:
		return type;
	case CHAR_ARRAY_TYPE:
		return type;
	default:
		return NO_TYPE;
	}
}

Type back_to_non_array_type(Type type)
{
	switch (type) {
	case INT_ARRAY_TYPE:
		return INT_TYPE;
	case FLOAT_ARRAY_TYPE:
		return FLOAT_TYPE;
	case CHAR_ARRAY_TYPE:
		return CHAR_TYPE;
	default:
		return type;
	}
}

AST *cast(AST * n)
{
	Type t = get_node_type(n);
	Unif unif = unify_other_arith(last_decl_type, t);
	if (unif.type == NO_TYPE) {
		type_error("cast", last_decl_type, t);
	}
	return create_conv_node(unif.rc, n);
}

AST *create_conv_node(Conv conv, AST * n)
{
	switch (conv) {
	case I2F:
		return new_subtree(I2F_NODE, FLOAT_TYPE, 1, n);
	case I2C:
		return new_subtree(I2C_NODE, CHAR_TYPE, 1, n);
	case F2I:
		return new_subtree(F2I_NODE, INT_TYPE, 1, n);
	case C2I:
		return new_subtree(C2I_NODE, INT_TYPE, 1, n);
	case NONE:
		return n;
	default:
		printf("INTERNAL ERROR: invalid conversion of types!\n");
		exit(EXIT_FAILURE);
	}
}

AST *unify_bin_node(AST * l, AST * r, NodeKind kind, const char *op,
		    Unif(*unify) (Type, Type))
{
	Type lt = get_node_type(l);
	Type rt = get_node_type(r);
	Unif unif = unify(lt, rt);
	if (unif.type == NO_TYPE) {
		type_error(op, lt, rt);
	}
	l = create_conv_node(unif.lc, l);
	r = create_conv_node(unif.rc, r);
	return new_subtree(kind, unif.type, 2, l, r);
}

AST *check_assign(AST * l, char *op, AST * r)
{
	Type lt = get_node_type(l);
	Type rt = get_node_type(r);

	if (lt == VOID_TYPE)
		type_error("=", lt, rt);
	if (lt == CHAR_TYPE) {
		if (rt == INT_TYPE)
			r = create_conv_node(I2C, r);
		else if (rt != CHAR_TYPE)
			type_error("=", lt, rt);
	}
	if (lt == INT_TYPE) {
		if (rt == CHAR_TYPE)
			r = create_conv_node(C2I, r);
		else if (rt == FLOAT_TYPE)
			r = create_conv_node(F2I, r);
		else if (rt != INT_TYPE)
			type_error("=", lt, rt);
	}
	if (lt == FLOAT_TYPE) {
		if (rt == INT_TYPE)
			r = create_conv_node(I2F, r);
		else if (rt != FLOAT_TYPE)
			type_error("=", lt, rt);
	}

	if (strcmp(op, "*=") == 0) {
		r = new_subtree(MUL_NODE, lt, 2, l, r);
		char *l_name = get_name(l);
		l = new_node(get_kind(l), get_data(l), get_node_type(l));
		set_name(l, l_name);
	} else if (strcmp(op, "/=") == 0) {
		r = new_subtree(DIV_NODE, lt, 2, l, r);
		char *l_name = get_name(l);
		l = new_node(get_kind(l), get_data(l), get_node_type(l));
		set_name(l, l_name);
	} else if (strcmp(op, "+=") == 0) {
		r = new_subtree(PLUS_NODE, lt, 2, l, r);
		char *l_name = get_name(l);
		l = new_node(get_kind(l), get_data(l), get_node_type(l));
		set_name(l, l_name);
	} else if (strcmp(op, "-=") == 0) {
		r = new_subtree(MINUS_NODE, lt, 2, l, r);
		char *l_name = get_name(l);
		l = new_node(get_kind(l), get_data(l), get_node_type(l));
		set_name(l, l_name);
	} else if (strcmp(op, "%=") == 0) {
		r = new_subtree(MOD_NODE, lt, 2, l, r);
		char *l_name = get_name(l);
		l = new_node(get_kind(l), get_data(l), get_node_type(l));
		set_name(l, l_name);
	} else if (strcmp(op, "<<=") == 0) {
		r = new_subtree(LEFT_NODE, lt, 2, l, r);
		char *l_name = get_name(l);
		l = new_node(get_kind(l), get_data(l), get_node_type(l));
		set_name(l, l_name);
	} else if (strcmp(op, ">>=") == 0) {
		r = new_subtree(RIGHT_NODE, lt, 2, l, r);
		char *l_name = get_name(l);
		l = new_node(get_kind(l), get_data(l), get_node_type(l));
		set_name(l, l_name);
	} else if (strcmp(op, "&=") == 0) {
		r = new_subtree(AND_NODE, lt, 2, l, r);
		char *l_name = get_name(l);
		l = new_node(get_kind(l), get_data(l), get_node_type(l));
		set_name(l, l_name);
	} else if (strcmp(op, "^=") == 0) {
		r = new_subtree(XOR_NODE, lt, 2, l, r);
		char *l_name = get_name(l);
		l = new_node(get_kind(l), get_data(l), get_node_type(l));
		set_name(l, l_name);
	} else if (strcmp(op, "|=") == 0) {
		r = new_subtree(OR_NODE, lt, 2, l, r);
		char *l_name = get_name(l);
		l = new_node(get_kind(l), get_data(l), get_node_type(l));
		set_name(l, l_name);
	}

	return new_subtree(ASSIGN_NODE, lt, 2, l, r);
}

void check_bool_expr(const char *cmd, AST * n)
{
	Type t = get_node_type(n);
	if (t != INT_TYPE) {
		printf
		    ("SEMANTIC ERROR (%d): conditional expression in '%s' is '%s' instead of '%s'.\n",
		     yylineno, cmd, get_text(t), get_text(INT_TYPE));
		exit(EXIT_FAILURE);
	}
}

AST *check_if_then(AST * e, AST * b)
{
	check_bool_expr("if", e);
	return new_subtree(IF_NODE, NO_TYPE, 2, e, b);
}

AST *check_if_then_else(AST * e, AST * b1, AST * b2)
{
	check_bool_expr("if", e);
	return new_subtree(IF_NODE, NO_TYPE, 3, e, b1, b2);
}

AST *check_while(AST * e, AST * b)
{
	check_bool_expr("while", e);
	return new_subtree(WHILE_NODE, NO_TYPE, 2, e, b);
}

AST *check_do(AST * e, AST * b)
{
	check_bool_expr("do", e);
	return new_subtree(DO_NODE, NO_TYPE, 2, b, e);
}

AST *check_for_AC(AST * a, AST * e, AST * b)
{
	check_bool_expr("for", e);
	return new_subtree(FOR_NODE, NO_TYPE, 3, a, e, b);
}

AST *check_for_ACI(AST * a, AST * e, AST * i, AST * b)
{
	check_bool_expr("for", e);
	return new_subtree(FOR_NODE, NO_TYPE, 4, a, e, i, b);
}

AST *check_for_DC(AST * d, AST * e, AST * b)
{
	check_bool_expr("for", e);
	return new_subtree(FOR_NODE, NO_TYPE, 3, d, e, b);
}

AST *check_for_DCI(AST * d, AST * e, AST * i, AST * b)
{
	check_bool_expr("for", e);
	return new_subtree(FOR_NODE, NO_TYPE, 4, d, e, i, b);
}
