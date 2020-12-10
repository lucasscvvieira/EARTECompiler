
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "tables.h"
#include "types.h"

#define CHILDREN_LIMIT 20	// Don't try this at home, kids... :P

struct node {
	NodeKind kind;
	union {
		int as_int;
		float as_float;
	} data;
	char *name;
	Type type;
	int count;
	AST *child[CHILDREN_LIMIT];
};

AST *new_node(NodeKind kind, int data, Type type)
{
	AST *node = malloc(sizeof *node);
	node->kind = kind;
	node->data.as_int = data;
	node->type = type;
	node->count = 0;
	for (int i = 0; i < CHILDREN_LIMIT; i++) {
		node->child[i] = NULL;
	}
	return node;
}

void set_name(AST * n, char *name)
{
	n->name = strdup(name);
}

char *get_name(AST * n)
{
	return n->name;
}

void add_child(AST * parent, AST * child)
{
	if (parent->count == CHILDREN_LIMIT) {
		fprintf(stderr, "Cannot add another child!\n");
		exit(1);
	}
	if (child == NULL)
		return;

	//if (get_child_count(child) == 0 && get_kind(child) == BLOCK_NODE) {
	//      free_tree(child);
	//      return;
	//}

	parent->child[parent->count] = child;
	parent->count++;
}

AST *get_child(AST * parent, int idx)
{
	return parent->child[idx];
}

AST *new_subtree(NodeKind kind, Type type, int child_count, ...)
{
	//printf("new_subtree %s %s\n", kind2str(kind), get_text(type));
	if (child_count > CHILDREN_LIMIT) {
		fprintf(stderr, "Too many children as arguments!\n");
		exit(1);
	}

	AST *node = new_node(kind, 0, type);
	va_list ap;
	va_start(ap, child_count);
	for (int i = 0; i < child_count; i++) {
		add_child(node, va_arg(ap, AST *));
	}
	va_end(ap);
	return node;
}

NodeKind get_kind(AST * node)
{
	return node->kind;
}

int get_data(AST * node)
{
	return node->data.as_int;
}

void set_float_data(AST * node, float data)
{
	node->data.as_float = data;
}

float get_float_data(AST * node)
{
	return node->data.as_float;
}

Type get_node_type(AST * node)
{
	return node->type;
}

void set_node_type(AST * node, Type t)
{
	node->type = t;
}

int get_child_count(AST * node)
{
	return node->count;
}

void free_tree(AST * tree)
{
	//printf("sfsd %s\n", kind2str(get_kind(tree)));
	if (tree == NULL)
		return;
	for (int i = 0; i < tree->count; i++) {
		free_tree(tree->child[i]);
	}
	if (tree->name != NULL)
		free(tree->name);
	free(tree);
}

// Dot output.

int nr;

extern varTable *vt;

char *kind2str(NodeKind kind)
{
	switch (kind) {
	case PROGRAM_NODE:
		return "program";
	case BLOCK_NODE:
		return "block";
	case ASSIGN_NODE:
		return ":=";
		// Operations
	case MINUS_NODE:
		return "-";
	case PLUS_NODE:
		return "+";
	case MUL_NODE:
		return "*";
	case DIV_NODE:
		return "/";
	case MOD_NODE:
		return "%";
	case LEFT_NODE:
		return "<<";
	case RIGHT_NODE:
		return ">>";
	case AND_NODE:
		return "&";
	case XOR_NODE:
		return "^";
	case OR_NODE:
		return "|";
		// Bool
	case LT_NODE:
		return "<";
	case LTE_NODE:
		return "<=";
	case GT_NODE:
		return ">";
	case GTE_NODE:
		return ">=";
	case EQ_NODE:
		return "==";
	case NE_NODE:
		return "!=";
		// Types
	case INT_VAL_NODE:
		return "";
	case FLOAT_VAL_NODE:
		return "";
	case CHAR_VAL_NODE:
		return "";
	case STR_VAL_NODE:
		return "";
		// Func
	case FUNC_DECL_NODE:
		return "func_decl";
	case FUNC_CALL_NODE:
		return "func_call";
		// Var
	case GLOBAL_VAR_DECL_NODE:
		return "global_var_decl";
	case GLOBAL_VAR_USE_NODE:
		return "global_var_use";
	case ARG_DECL_NODE:
		return "arg_decl";
	case ARG_USE_NODE:
		return "arg_use";
	case VAR_DECL_NODE:
		return "var_decl";
	case VAR_USE_NODE:
		return "var_use";
	case VAR_LIST_NODE:
		return "var_list";
		//
	case READ_NODE:
		return "read";
	case WRITE_NODE:
		return "write";
		//
	case WHILE_NODE:
		return "while";
	case FOR_NODE:
		return "for";
	case DO_NODE:
		return "do";
	case IF_NODE:
		return "if";

	case I2F_NODE:
		return "I2F";
	case I2C_NODE:
		return "I2C";
	case F2I_NODE:
		return "F2I";
	case C2I_NODE:
		return "C2I";

	default:
		return "ERROR!!";
	}
}

int has_data(NodeKind kind)
{
	switch (kind) {
	case INT_VAL_NODE:
	case FLOAT_VAL_NODE:
	case STR_VAL_NODE:
	case CHAR_VAL_NODE:
	case VAR_DECL_NODE:
	case VAR_USE_NODE:
	case GLOBAL_VAR_DECL_NODE:
	case GLOBAL_VAR_USE_NODE:
	case ARG_USE_NODE:
		return 1;
	default:
		return 0;
	}
}

int print_node_dot(AST * node)
{
	int my_nr = nr++;

	fprintf(stderr, "node%d[label=\"", my_nr);
	if (node->type != NO_TYPE) {
		fprintf(stderr, "(%s) ", get_text(node->type));
	}
	if (node->kind == VAR_DECL_NODE || node->kind == VAR_USE_NODE
	    || node->kind == GLOBAL_VAR_DECL_NODE
	    || node->kind == GLOBAL_VAR_USE_NODE || node->kind == ARG_DECL_NODE
	    || node->kind == ARG_USE_NODE) {
		fprintf(stderr, "%s@", node->name);
	} else {
		fprintf(stderr, "%s", kind2str(node->kind));
	}
	if (has_data(node->kind)) {
		if (node->kind == FLOAT_VAL_NODE) {
			fprintf(stderr, "%.2f", node->data.as_float);
		} else if (node->kind == STR_VAL_NODE) {
			fprintf(stderr, "@%d", node->data.as_int);
		} else if (node->kind == CHAR_VAL_NODE) {
			fprintf(stderr, "@%d", node->data.as_int);
		} else {
			fprintf(stderr, "%d", node->data.as_int);
		}
	}
	fprintf(stderr, "\"];\n");

	for (int i = 0; i < node->count; i++) {
		int child_nr = print_node_dot(node->child[i]);
		fprintf(stderr, "node%d -> node%d;\n", my_nr, child_nr);
	}
	return my_nr;
}

void print_dot(AST * tree)
{
	nr = 0;
	fprintf(stderr, "digraph {\ngraph [ordering=\"out\"];\n");
	print_node_dot(tree);
	fprintf(stderr, "}\n");
}
