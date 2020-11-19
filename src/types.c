
#include "types.h"

static const char *TYPE_STRING[] = {
	"void",
	"int",
	"int[]",
	"float",
	"float[]",
	"char",
	"char[]",
};

const char *get_text(Type type)
{
	return TYPE_STRING[type];
}

static const Type plus[7][7] = {
    // *INDENT-OFF*
                        //VOID_TYPE INT_TYPE    INT_ARRAY_TYPE  FLOAT_TYPE  FLOAT_ARRAY_TYPE  CHAR_TYPE  CHAR_ARRAY_TYPE
    /*VOID_TYPE*/       { NO_TYPE,  NO_TYPE,    NO_TYPE,        NO_TYPE,    NO_TYPE,          NO_TYPE,   NO_TYPE },
    /*INT_TYPE*/        { NO_TYPE,  INT_TYPE,   NO_TYPE,        CHAR_TYPE,  NO_TYPE,          INT_TYPE,  NO_TYPE },
    /*INT_ARRAY_TYPE*/  { NO_TYPE,  NO_TYPE,    INT_ARRAY_TYPE, NO_TYPE,    NO_TYPE,          NO_TYPE,   NO_TYPE },
    /*FLOAT_TYPE*/      { NO_TYPE,  FLOAT_TYPE, NO_TYPE,        FLOAT_TYPE, NO_TYPE,          NO_TYPE,   NO_TYPE },
    /*FLOAT_ARRAY_TYPE*/{ NO_TYPE,  NO_TYPE,    NO_TYPE,        NO_TYPE,    FLOAT_ARRAY_TYPE, NO_TYPE,   NO_TYPE },
    /*CHAR_TYPE*/       { NO_TYPE,  CHAR_TYPE,  NO_TYPE,        CHAR_TYPE,  NO_TYPE,          CHAR_TYPE, NO_TYPE },
    /*CHAR_ARRAY_TYPE*/ { NO_TYPE,  NO_TYPE,    NO_TYPE,        NO_TYPE,    NO_TYPE,          NO_TYPE,   NO_TYPE }
    // *INDENT-ON*
};

Type unify_plus(Type lt, Type rt)
{
	return plus[lt][rt];
}

static const Type other[7][7] = {
    // *INDENT-OFF*
                        //VOID_TYPE INT_TYPE    INT_ARRAY_TYPE FLOAT_TYPE  FLOAT_ARRAY_TYPE CHAR_TYPE  CHAR_ARRAY_TYPE
    /*VOID_TYPE*/       { NO_TYPE,  NO_TYPE,    NO_TYPE,       NO_TYPE,    NO_TYPE,         NO_TYPE,   NO_TYPE },
    /*INT_TYPE*/        { NO_TYPE,  INT_TYPE,   NO_TYPE,       FLOAT_TYPE, NO_TYPE,         INT_TYPE,  NO_TYPE },
    /*INT_ARRAY_TYPE*/  { NO_TYPE,  NO_TYPE,    NO_TYPE,       NO_TYPE,    NO_TYPE,         NO_TYPE,   NO_TYPE },
    /*FLOAT_TYPE*/      { NO_TYPE,  FLOAT_TYPE, NO_TYPE,       FLOAT_TYPE, NO_TYPE,         FLOAT_TYPE,NO_TYPE },
    /*FLOAT_ARRAY_TYPE*/{ NO_TYPE,  NO_TYPE,    NO_TYPE,       NO_TYPE,    NO_TYPE,         NO_TYPE,   NO_TYPE },
    /*CHAR_TYPE*/       { NO_TYPE,  CHAR_TYPE,  NO_TYPE,       FLOAT_TYPE, NO_TYPE,         CHAR_TYPE, NO_TYPE },
    /*CHAR_ARRAY_TYPE*/ { NO_TYPE,  NO_TYPE,    NO_TYPE,       NO_TYPE,    NO_TYPE,         NO_TYPE,   NO_TYPE }
    // *INDENT-ON*
};

Type unify_other_arith(Type lt, Type rt)
{
	return other[lt][rt];
}

static const Type comp[7][7] = {
    // *INDENT-OFF*
                        //VOID_TYPE INT_TYPE  INT_ARRAY_TYPE FLOAT_TYPE FLOAT_ARRAY_TYPE CHAR_TYPE CHAR_ARRAY_TYPE
    /*VOID_TYPE*/       { NO_TYPE,  NO_TYPE,  NO_TYPE,       NO_TYPE,   NO_TYPE,         NO_TYPE,  NO_TYPE },
    /*INT_TYPE*/        { NO_TYPE,  INT_TYPE, NO_TYPE,       INT_TYPE,  NO_TYPE,         INT_TYPE, NO_TYPE },
    /*INT_ARRAY_TYPE*/  { NO_TYPE,  NO_TYPE,  NO_TYPE,       NO_TYPE,   NO_TYPE,         NO_TYPE,  NO_TYPE },
    /*FLOAT_TYPE*/      { NO_TYPE,  INT_TYPE, NO_TYPE,       INT_TYPE,  NO_TYPE,         INT_TYPE, NO_TYPE },
    /*FLOAT_ARRAY_TYPE*/{ NO_TYPE,  NO_TYPE,  NO_TYPE,       NO_TYPE,   NO_TYPE,         NO_TYPE,  NO_TYPE },
    /*CHAR_TYPE*/       { NO_TYPE,  INT_TYPE, NO_TYPE,       INT_TYPE,  NO_TYPE,         INT_TYPE, NO_TYPE },
    /*CHAR_ARRAY_TYPE*/ { NO_TYPE,  NO_TYPE,  NO_TYPE,       NO_TYPE,   NO_TYPE,         NO_TYPE,  NO_TYPE }
    // *INDENT-ON*
};

Type unify_comp(Type lt, Type rt)
{
	return comp[lt][rt];
}

static const Type logic[7][7] = {
    // *INDENT-OFF*
                        //VOID_TYPE INT_TYPE  INT_ARRAY_TYPE FLOAT_TYPE FLOAT_ARRAY_TYPE CHAR_TYPE CHAR_ARRAY_TYPE
    /*VOID_TYPE*/       { NO_TYPE,  NO_TYPE,  NO_TYPE,       NO_TYPE,   NO_TYPE,         NO_TYPE,  NO_TYPE },
    /*INT_TYPE*/        { NO_TYPE,  INT_TYPE, NO_TYPE,       NO_TYPE,   NO_TYPE,         INT_TYPE, NO_TYPE },
    /*INT_ARRAY_TYPE*/  { NO_TYPE,  NO_TYPE,  NO_TYPE,       NO_TYPE,   NO_TYPE,         NO_TYPE,  NO_TYPE },
    /*FLOAT_TYPE*/      { NO_TYPE,  NO_TYPE,  NO_TYPE,       NO_TYPE,   NO_TYPE,         NO_TYPE,  NO_TYPE },
    /*FLOAT_ARRAY_TYPE*/{ NO_TYPE,  NO_TYPE,  NO_TYPE,       NO_TYPE,   NO_TYPE,         NO_TYPE,  NO_TYPE },
    /*CHAR_TYPE*/       { NO_TYPE,  INT_TYPE, NO_TYPE,       NO_TYPE,   NO_TYPE,         INT_TYPE, NO_TYPE },
    /*CHAR_ARRAY_TYPE*/ { NO_TYPE,  NO_TYPE,  NO_TYPE,       NO_TYPE,   NO_TYPE,         NO_TYPE,  NO_TYPE }
    // *INDENT-ON*
};

Type unify_logic(Type lt, Type rt)
{
	return logic[lt][rt];
}
