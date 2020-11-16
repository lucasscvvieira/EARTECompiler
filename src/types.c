
#include "types.h"

static const char *TYPE_STRING[] = {
    "void",
    "int",
    "float",
    "char",
    "bool",
};

const char     *
get_text(Type type)
{
    return TYPE_STRING[type];
}

static const Type plus[5][5] = {
    // VOID_TYPE INT_TYPE FLOAT_TYPE CHAR_TYPE BOOL_TYPE
    /*
     * VOID_TYPE
     */ {NO_TYPE, NO_TYPE, NO_TYPE, NO_TYPE, NO_TYPE},
    /*
     * INT_TYPE
     */ {NO_TYPE, INT_TYPE, FLOAT_TYPE, CHAR_TYPE, BOOL_TYPE},
    /*
     * FLOAT_TYP
     */ {NO_TYPE, FLOAT_TYPE, FLOAT_TYPE, CHAR_TYPE, FLOAT_TYPE},
    /*
     * CHAR_TYPE
     */ {NO_TYPE, CHAR_TYPE, CHAR_TYPE, CHAR_TYPE, CHAR_TYPE},
    /*
     * BOOL_TYPE
     */ {NO_TYPE, INT_TYPE, FLOAT_TYPE, CHAR_TYPE, BOOL_TYPE}
};

Type
unify_plus(Type lt, Type rt)
{
    return plus[lt][rt];
}

static const Type other[5][5] = {
    // VOID_TYPE INT_TYPE FLOAT_TYPE CHAR_TYPE BOOL_TYPE
    /*
     * VOID_TYPE
     */ {NO_TYPE, NO_TYPE, NO_TYPE, NO_TYPE, NO_TYPE},
    /*
     * INT_TYPE
     */ {NO_TYPE, INT_TYPE, FLOAT_TYPE, NO_TYPE, NO_TYPE},
    /*
     * FLOAT_TYP
     */ {NO_TYPE, FLOAT_TYPE, FLOAT_TYPE, NO_TYPE, NO_TYPE},
    /*
     * CHAR_TYPE
     */ {NO_TYPE, NO_TYPE, NO_TYPE, NO_TYPE, NO_TYPE},
    /*
     * BOOL_TYPE
     */ {NO_TYPE, NO_TYPE, NO_TYPE, NO_TYPE, NO_TYPE}
};

Type
unify_other_arith(Type lt, Type rt)
{
    return other[lt][rt];
}

static const Type comp[5][5] = {
    // VOID_TYPE INT_TYPE FLOAT_TYPE CHAR_TYPE BOOL_TYPE
    /*
     * VOID_TYPE
     */ {NO_TYPE, NO_TYPE, NO_TYPE, NO_TYPE, NO_TYPE},
    /*
     * INT_TYPE
     */ {NO_TYPE, BOOL_TYPE, BOOL_TYPE, NO_TYPE, NO_TYPE},
    /*
     * FLOAT_TYP
     */ {NO_TYPE, BOOL_TYPE, BOOL_TYPE, NO_TYPE, NO_TYPE},
    /*
     * CHAR_TYPE
     */ {NO_TYPE, NO_TYPE, NO_TYPE, NO_TYPE, NO_TYPE},
    /*
     * BOOL_TYPE
     */ {NO_TYPE, NO_TYPE, NO_TYPE, NO_TYPE, BOOL_TYPE}
};

Type
unify_comp(Type lt, Type rt)
{
    return comp[lt][rt];
}
