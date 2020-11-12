
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tables.h"

extern char    *yytext;
extern int      yylineno;
extern Type     last_decl_type;

// Strings Table
// ----------------------------------------------------------------------------

#define STRING_MAX_SIZE 128
#define STRINGS_TABLE_MAX_SIZE 100

struct str_table {
    char            t[STRINGS_TABLE_MAX_SIZE][STRING_MAX_SIZE];
    int             size;
};

strTable       *
strTable_create()
{
    strTable       *st = malloc(sizeof *st);
    st->size = 0;
    return st;
}

int
strTable_add(strTable * st, char *s)
{
    for (int i = 0; i < st->size; i++) {
	if (strcmp(st->t[i], s) == 0) {
	    return i;
	}
    }
    strcpy(st->t[st->size], s);
    int             idx_added = st->size;
    st->size++;
    return idx_added;
}

char           *
strTable_get(strTable * st, int i)
{
    return st->t[i];
}

void
strTable_print(strTable * st)
{
    printf("Strings table:\n");
    for (int i = 0; i < st->size; i++) {
	printf("Entry %d -- %s\n", i, strTable_get(st, i));
    }
}

void
strTable_free(strTable * st)
{
    free(st);
}

// Variables Table
// ----------------------------------------------------------------------------

#define VARIABLE_MAX_SIZE 128
#define VARIABLES_TABLE_MAX_SIZE 100

typedef struct {
    char            name[VARIABLE_MAX_SIZE];
    int             line;
    Type            type;
} varTable_entry;

struct var_table {
    varTable_entry  t[VARIABLES_TABLE_MAX_SIZE];
    int             size;
};

varTable       *
varTable_create()
{
    varTable       *vt = malloc(sizeof *vt);
    vt->size = 0;
    return vt;
}

int
varTable_lookup(varTable * vt, char *s)
{
    for (int i = 0; i < vt->size; i++) {
	if (strcmp(vt->t[i].name, s) == 0) {
	    return i;
	}
    }
    return -1;
}

int
varTable_add(varTable * vt, char *s, int line, Type type)
{
    strcpy(vt->t[vt->size].name, s);
    vt->t[vt->size].line = line;
    vt->t[vt->size].type = type;
    int             idx_added = vt->size;
    vt->size++;
    return idx_added;
}

char           *
varTable_get_name(varTable * vt, int i)
{
    return vt->t[i].name;
}

int
varTable_get_line(varTable * vt, int i)
{
    return vt->t[i].line;
}

Type
varTable_get_type(varTable * vt, int i)
{
    return vt->t[i].type;
}

void
varTable_print(varTable * vt)
{
    printf("Variables table:\n");
    for (int i = 0; i < vt->size; i++) {
	printf("Entry %d -- name: %s, line: %d, type: %s\n", i,
	       varTable_get_name(vt, i), varTable_get_line(vt, i),
	       get_text(varTable_get_type(vt, i)));
    }
}

void
varTable_free(varTable * vt)
{
    free(vt);
}

// Functions Table
// ----------------------------------------------------------------------------

#define FUNCTION_MAX_SIZE 128
#define FUNCTIONS_TABLE_MAX_SIZE 100

typedef struct {
    char            name[FUNCTION_MAX_SIZE];
    int             line;
    varTable       *vt;
    varTable       *argT;
    Type            type;
} funcTable_entry;

struct func_table {
    funcTable_entry t[FUNCTIONS_TABLE_MAX_SIZE];
    int             size;
};

funcTable      *
funcTable_create()
{
    funcTable      *ft = malloc(sizeof *ft);
    ft->size = 0;
    return ft;
}

int
funcTable_lookup(funcTable * ft, char *s)
{
    for (int i = 0; i < ft->size; i++) {
	if (strcmp(ft->t[i].name, s) == 0) {
	    return i;
	}
    }
    return -1;
}

int
funcTable_add(funcTable * ft, char *s, int line, Type type)
{
    strcpy(ft->t[ft->size].name, s);
    ft->t[ft->size].line = line;
    ft->t[ft->size].type = type;
    ft->t[ft->size].vt = varTable_create();
    ft->t[ft->size].argT = varTable_create();
    int             idx_added = ft->size;
    ft->size++;
    return idx_added;
}

char           *
funcTable_get_name(funcTable * ft, int i)
{
    return ft->t[i].name;
}

int
funcTable_get_line(funcTable * ft, int i)
{
    return ft->t[i].line;
}

varTable       *
funcTable_get_args(funcTable * ft, int i)
{
    return ft->t[i].argT;
}

varTable       *
funcTable_get_vt(funcTable * ft, int i)
{
    return ft->t[i].vt;
}

Type
funcTable_get_type(funcTable * ft, int i)
{
    return ft->t[i].type;
}

void
funcTable_print(funcTable * ft)
{
    printf("Functions table:\n");
    for (int i = 0; i < ft->size; i++) {
	printf("Entry %d -- name: %s, line: %d, type: %s\n", i,
	       funcTable_get_name(ft, i), funcTable_get_line(ft, i),
	       get_text(funcTable_get_type(ft, i)));
    }
}

void
funcTable_free(funcTable * ft)
{
    free(ft);
}
