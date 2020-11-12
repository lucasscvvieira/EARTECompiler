#include <stdio.h>

#include "parser.h"
#include "tables.h"

extern int      yylex_destroy(void);

strTable       *st;
varTable       *vt;
funcTable      *ft;

int
main(void)
{
    st = strTable_create();
    vt = varTable_create();
    ft = funcTable_create();

    if (yyparse() == 0)
	printf("PARSE SUCCESSFUL!\n");

    /*
     * printf("\n\n"); strTable_print(st); printf("\n\n");
     * varTable_print(vt); printf("\n\n");
     * varTable_print(funcTable_get_args(ft, funcTable_lookup(ft,
     * "main"))); printf("\n\n"); varTable_print(funcTable_get_vt(ft,
     * funcTable_lookup(ft, "main"))); printf("\n\n");
     * funcTable_print(ft); printf("\n\n"); 
     */

    strTable_free(st);
    varTable_free(vt);
    funcTable_free(ft);
    yylex_destroy();

    return 0;
}
