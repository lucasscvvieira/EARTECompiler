#ifndef UTILS_H
#define UTILS_H

#include "types.h"
#include "ast.h"

AST *check_func();

AST *new_func();

void check_return(Type type);

void new_arg();

AST *check_arg();

AST *check_var();

AST *new_var();

void type_error(const char *op, Type t1, Type t2);	//

Type get_array_type(Type type);
Type back_to_non_array_type(Type type);

AST *cast(AST * n);
AST *create_conv_node(Conv conv, AST * n);

AST *unify_bin_node(AST * l, AST * r, NodeKind kind, const char *op,
		    Unif(*unify) (Type, Type));

AST *check_assign(AST * l, char *op, AST * r);

void check_bool_expr(const char *cmd, AST * n);

AST *check_if_then(AST * e, AST * b);
AST *check_if_then_else(AST * e, AST * b1, AST * b2);
AST *check_for(AST * b, AST * e);
AST *check_while(AST * b, AST * e);
AST *check_do(AST * b, AST * e);
#endif
