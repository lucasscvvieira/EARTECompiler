#include <stdio.h>
#include "parser.h"

int yylex_destroy(void);

int
main(void)
{
    st = create_str_table();
    vt = create_var_table();

    if (yyparse() == 0)
	printf("PARSE SUCCESSFUL!\n");
    else
	printf("PARSE FAILED!\n");
    
    printf("\n\n");
    print_str_table(st); printf("\n\n");
    print_var_table(vt); printf("\n\n");

    free_str_table(st);
    free_var_table(vt);
    yylex_destroy();

    return 0;
}
