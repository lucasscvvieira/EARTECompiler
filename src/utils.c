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

void
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
