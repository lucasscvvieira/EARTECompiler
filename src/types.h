
#ifndef TYPES_H
#define TYPES_H

typedef enum {
    VOID_TYPE,
    INT_TYPE,
    FLOAT_TYPE,
    CHAR_TYPE,
    BOOL_TYPE,
    NO_TYPE			// non-existing type to a function.
} Type;

const char     *get_text(Type type);

Type            unify_plus(Type lt, Type rt);
Type            unify_other_arith(Type lt, Type rt);
Type            unify_comp(Type lt, Type rt);

#endif				// TYPES_H
