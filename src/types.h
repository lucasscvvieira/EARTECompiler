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
	NO_TYPE			// non-existing type to a function.
} Type;

const char *get_text(Type type);

typedef enum {			// Basic conversions between types.
	I2F,
	I2C,
	F2I,
	C2I,
	NONE
} Conv;

typedef struct {
	Type type;
	Conv lc;		// Left conversion.
	Conv rc;		// Right conversion.
} Unif;

Unif unify_plus(Type lt, Type rt);
Unif unify_other_arith(Type lt, Type rt);
Unif unify_comp(Type lt, Type rt);
Unif unify_logic(Type lt, Type rt);
Unif unify_shift(Type lt, Type rt);

#endif
