#include <stdio.h>
#include "parser.h"

int
main(void)
{
    if (yyparse() == 0)
	printf("PARSE SUCCESSFUL!\n");
    else
	printf("PARSE FAILED!\n");
    return 0;
}
