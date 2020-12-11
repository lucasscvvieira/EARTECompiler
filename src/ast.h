#ifndef AST_H
#define AST_H

#include "types.h"

typedef enum {
	PROGRAM_NODE,
	BLOCK_NODE,
	ASSIGN_NODE,
	// Operations
	MINUS_NODE,
	PLUS_NODE,
	MUL_NODE,
	DIV_NODE,
	MOD_NODE,
	LEFT_NODE,
	RIGHT_NODE,
	AND_NODE,
	XOR_NODE,
	OR_NODE,
	// Bool
	LT_NODE,
	LTE_NODE,
	GT_NODE,
	GTE_NODE,
	EQ_NODE,
	NE_NODE,
	// Types
	INT_VAL_NODE,
	FLOAT_VAL_NODE,
	CHAR_VAL_NODE,
	STR_VAL_NODE,
	// Func
	FUNC_DECL_NODE,
	FUNC_CALL_NODE,
	// Var
	GLOBAL_VAR_DECL_NODE,
	GLOBAL_VAR_USE_NODE,
	ARG_DECL_NODE,
	ARG_USE_NODE,
	VAR_DECL_NODE,
	VAR_USE_NODE,
	VAR_LIST_NODE,
	//
	RETURN_NODE,
	BREAK_NODE,
	//
	WHILE_NODE,
	FOR_NODE,
	DO_NODE,
	IF_NODE,

	I2F_NODE,		// Conversion of types.
	I2C_NODE,
	F2I_NODE,
	C2I_NODE
} NodeKind;

struct node;			// Opaque structure to ensure encapsulation.

typedef struct node AST;

AST *new_node(NodeKind kind, int data, Type type);

void set_name(AST * n, char *name);
char *get_name(AST * n);
void add_child(AST * parent, AST * child);
AST *get_child(AST * parent, int idx);

AST *new_subtree(NodeKind kind, Type type, int child_count, ...);

NodeKind get_kind(AST * node);
char *kind2str(NodeKind kind);

int get_data(AST * node);
void set_float_data(AST * node, float data);
float get_float_data(AST * node);

Type get_node_type(AST * node);
void set_node_type(AST * node, Type t);
int get_child_count(AST * node);

void print_tree(AST * ast);
void print_dot(AST * ast);

void free_tree(AST * ast);

#endif
