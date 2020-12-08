#ifndef UTILS_H
#define UTILS_H

#include "types.h"

Type check_func();

void new_func();

void check_return(Type type);

void new_arg();

void check_arg();

Type check_var();

void new_var();

void type_error(const char *op, Type t1, Type t2);	//

Type get_array_type(Type type);
Type back_to_non_array_type(Type type);

Type unify_bin_op(Type l, Type r, const char *op, Type(*unify) (Type, Type));

void check_assign(Type l, Type r);

void check_bool_expr(const char *cmd, Type t);

#endif
