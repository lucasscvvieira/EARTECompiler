#include <stdio.h>

#include "tables.h"
#include "bison.h"
#include "parser.h"

extern int yylex_destroy(void);

strTable *st;
varTable *vt;
funcTable *ft;

AST *root;

int main(void)
{
	st = strTable_create();
	vt = varTable_create();
	ft = funcTable_create();

	// IO Functions
	// Input Functions
	funcTable_add(ft, "readInt", 0, INT_TYPE);
	funcTable_add(ft, "readFloat", 0, FLOAT_TYPE);
	funcTable_add(ft, "readChar", 0, CHAR_TYPE);
	funcTable_add(ft, "readString", 0, CHAR_ARRAY_TYPE);

	// Output Functions
	int printIntID = funcTable_add(ft, "printInt", 0, VOID_TYPE);
	varTable *printIntArgs = funcTable_get_args(ft, printIntID);
	varTable_add(printIntArgs, "var", 0, INT_TYPE);

	int printFloatID = funcTable_add(ft, "printFloat", 0, VOID_TYPE);
	varTable *printFloatArgs = funcTable_get_args(ft, printFloatID);
	varTable_add(printFloatArgs, "var", 0, FLOAT_TYPE);

	int printCharID = funcTable_add(ft, "printChar", 0, VOID_TYPE);
	varTable *printCharArgs = funcTable_get_args(ft, printCharID);
	varTable_add(printCharArgs, "var", 0, CHAR_TYPE);

	int printStringID = funcTable_add(ft, "printString", 0, VOID_TYPE);
	varTable *printStringArgs = funcTable_get_args(ft, printStringID);
	varTable_add(printStringArgs, "var", 0, CHAR_ARRAY_TYPE);

	if (yyparse() == 0)
		printf("PARSE SUCCESSFUL!\n");

	fprintf(stderr, "\n\n");
	print_dot(root);

	fprintf(stderr, "\n\n");
	fprintf(stderr, "Tables Data:\n");
	fprintf(stderr, "Strings:\n");
	strTable_print(st);
	fprintf(stderr, "\n\n");
	fprintf(stderr, "Global Variables:\n");
	varTable_print(vt);
	fprintf(stderr, "\n\n");
	funcTable_print(ft);
	fprintf(stderr, "\n\n");

	strTable_free(st);
	varTable_free(vt);
	funcTable_free(ft);
	free_tree(root);
	yylex_destroy();

	return 0;
}
