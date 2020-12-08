#ifndef TYPES_H
#define TYPES_H

typedef enum {
	VOID_TYPE,
	INT_TYPE,
	INT_ARRAY_TYPE,
	FLOAT_TYPE,
	FLOAT_ARRAY_TYPE,
	CHAR_TYPE,
	CHAR_ARRAY_TYPE,
	NO_TYPE,		// non-existing type to a function.
} Type;

const char *get_text(Type type);

Type unify_plus(Type lt, Type rt);
Type unify_other_arith(Type lt, Type rt);
Type unify_comp(Type lt, Type rt);
Type unify_logic(Type lt, Type rt);

#endif
