#include <stdio.h>

#include "tables.h"
#include "bison.h"
#include "parser.h"

extern int yylex_destroy(void);

strTable *st;
varTable *vt;
funcTable *ft;

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
/*
	printf("\n\n");
	strTable_print(st);
	printf("\n\n");
	varTable_print(vt);
	printf("\n\n");
	varTable_print(funcTable_get_args(ft, funcTable_lookup(ft, "main")));
	printf("\n\n");
	varTable_print(funcTable_get_vt(ft, funcTable_lookup(ft, "main")));
	printf("\n\n");
	funcTable_print(ft);
	printf("\n\n");
*/
	strTable_free(st);
	varTable_free(vt);
	funcTable_free(ft);
	yylex_destroy();

	return 0;
}
