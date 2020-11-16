#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

#include "types.h"
#include "tables.h"

extern int      yylineno;
extern varTable *vt;
extern funcTable *ft;
extern char    *last_decl_func;
extern Type     last_decl_type;

void
check_func()
{
    int             idx = funcTable_lookup(ft, yylval.sval);
    if (idx == -1) {
	printf("SEMANTIC ERROR (%d): function '%s' was not declared.\n",
	       yylineno, yylval.sval);
	exit(EXIT_FAILURE);
    }

	return funcTable_get_type(ft, idx);
}

void
new_func()
{
    int             idx = funcTable_lookup(ft, yylval.sval);

    if (idx != -1) {
	printf
	    ("SEMANTIC ERROR (%d): function '%s' already declared at line %d.\n",
	     yylineno, yylval.sval, funcTable_get_line(ft, idx));
	exit(EXIT_FAILURE);
    }
    // Updates the auxiliary variable with the defined function name
    last_decl_func = strdup(yylval.sval);
    funcTable_add(ft, yylval.sval, yylineno, last_decl_type);
}

void
new_arg()
{
    if (last_decl_func == NULL) {
	printf
	    ("SEMANTIC ERROR (%d): argument '%s' for undeclared function.\n",
	     yylineno, yylval.sval);
	exit(EXIT_FAILURE);
    }
    varTable       *args = funcTable_get_args(ft,
					      funcTable_lookup(ft,
							       last_decl_func)
	);

    int             idx = varTable_lookup(args, yylval.sval);
    if (idx != -1) {
	printf
	    ("SEMANTIC ERROR (%d): argument '%s' already declared at line %d.\n",
	     yylineno, yylval.sval, varTable_get_line(args, idx));
	exit(EXIT_FAILURE);
    }

    varTable_add(args, yylval.sval, yylineno, last_decl_type);
}

Type
check_var()
{
    int             idx = -1;
    varTable       *aux = NULL;

    if (last_decl_func != NULL) {
	// Verify if variable is a function argument
	aux = funcTable_get_args(ft, funcTable_lookup(ft, last_decl_func)
	    );
	idx = varTable_lookup(aux, yylval.sval);

	// If not found, verify if variable is a function variable
	if (idx == -1) {
	    aux = funcTable_get_vt(ft, funcTable_lookup(ft, last_decl_func)
		);
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

	return varTable_get_line(vt, idx);
}

void
new_var()
{
    int             idx = -1;
    varTable       *aux = NULL;

    if (last_decl_func != NULL) {
	// Verify if variable is a function argument
	aux = funcTable_get_args(ft, funcTable_lookup(ft, last_decl_func)
	    );
	idx = varTable_lookup(aux, yylval.sval);

	// If not found, verify if variable is a function variable
	if (idx == -1) {
	    aux = funcTable_get_vt(ft, funcTable_lookup(ft, last_decl_func)
		);
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

    if (last_decl_func != NULL) {
	varTable_add(funcTable_get_vt(ft,
				      funcTable_lookup(ft, last_decl_func)
		     ), yylval.sval, yylineno, last_decl_type);
    } else
	varTable_add(vt, yylval.sval, yylineno, last_decl_type);
}

// ----------------------------------------------------------------------------

// Type checking and inference.

void type_error(const char* op, Type t1, Type t2) {
    printf("SEMANTIC ERROR (%d): incompatible types for operator '%s', LHS is '%s' and RHS is '%s'.\n",
           yylineno, op, get_text(t1), get_text(t2));
    exit(EXIT_FAILURE);
}

Type unify_bin_op(Type l, Type r,
                  const char* op, Type (*unify)(Type,Type)) {
    Type unif = unify(l, r);
    if (unif == NO_TYPE) {
        type_error(op, l, r);
    }
    return unif;
}

void check_assign(Type l, Type r) {
	if (l == VOID_TYPE) type_error(":=", l, r);
    if (l == BOOL_TYPE && r != BOOL_TYPE) type_error(":=", l, r);
    if (l == CHAR_TYPE && r != CHAR_TYPE)  type_error(":=", l, r);
    if (l == INT_TYPE  && r != INT_TYPE)  type_error(":=", l, r);
    if (l == FLOAT_TYPE && !(r == INT_TYPE || r == FLOAT_TYPE)) type_error(":=", l, r);
}

void check_bool_expr(const char* cmd, Type t) {
    if (t != BOOL_TYPE) {
        printf("SEMANTIC ERROR (%d): conditional expression in '%s' is '%s' instead of '%s'.\n",
           yylineno, cmd, get_text(t), get_text(BOOL_TYPE));
        exit(EXIT_FAILURE);
    }
}