#ifndef TABLES_H
#define TABLES_H

#include "types.h"

// Strings Table
// ----------------------------------------------------------------------------

// Opaque structure.
// For simplicity, the table is implemented as a sequential list.
struct str_table;
typedef struct str_table strTable;

// Creates an empty strings table.
strTable       *strTable_create();

// Adds the given string to the table without repetitions.
// String 's' is copied internally.
// Returns the index of the string in the table.
int             strTable_add(strTable * st, char *s);

// Returns a pointer to the string stored at index 'i'.
char           *strTable_get(strTable * st, int i);

// Prints the given table to stdout.
void            strTable_print(strTable * st);

// Clears the allocated structure.
void            strTable_free(strTable * st);

// Variables Table
// ----------------------------------------------------------------------------

// Opaque structure.
// For simplicity, the table is implemented as a sequential list.
// This table only stores the variable name and type, and its declaration
// line.
struct var_table;
typedef struct var_table varTable;

// Creates an empty variables table.
varTable       *varTable_create();

// Adds a fresh var to the table.
// No check is made by this function, so make sure to call 'lookup_var'
// first.
// Returns the index where the variable was inserted.
int             varTable_add(varTable * vt, char *s, int line, Type type);

// Returns the index where the given variable is stored or -1 otherwise.
int             varTable_lookup(varTable * vt, char *s);

// Returns the variable name stored at the given index.
// No check is made by this function, so make sure that the index is valid
// first.
char           *varTable_get_name(varTable * vt, int i);

// Returns the declaration line of the variable stored at the given index.
// No check is made by this function, so make sure that the index is valid
// first.
int             varTable_get_line(varTable * vt, int i);

// Returns the variable type stored at the given index.
// No check is made by this function, so make sure that the index is valid
// first.
Type            varTable_get_type(varTable * vt, int i);

// Returns the variable type stored at the given index.
// No check is made by this function, so make sure that the index is valid
// first.
char           *varTable_get_func(varTable * vt, int i);

// Returns the variable type stored at the given index.
// No check is made by this function, so make sure that the index is valid
// first.
int             varTable_get_namespace(varTable * vt, int i);

// Prints the given table to stdout.
void            varTable_print(varTable * vt);

// Clears the allocated structure.
void            varTable_free(varTable * vt);

// Functions Table
// ----------------------------------------------------------------------------

// Opaque structure.
// For simplicity, the table is implemented as a sequential list.
// This table only stores the variable name and type, and its declaration
// line.
struct func_table;
typedef struct func_table funcTable;

// Creates an empty variables table.
funcTable      *funcTable_create();

// Adds a fresh var to the table.
// No check is made by this function, so make sure to call 'lookup_var'
// first.
// Returns the index where the variable was inserted.
int             funcTable_add(funcTable * vt, char *s, int line,
			      Type type);

// Returns the index where the given variable is stored or -1 otherwise.
int             funcTable_lookup(funcTable * vt, char *s);

// Returns the variable name stored at the given index.
// No check is made by this function, so make sure that the index is valid
// first.
char           *funcTable_get_name(funcTable * vt, int i);

// Returns the declaration line of the variable stored at the given index.
// No check is made by this function, so make sure that the index is valid
// first.
int             funcTable_get_line(funcTable * vt, int i);

// Returns the declaration line of the variable stored at the given index.
// No check is made by this function, so make sure that the index is valid
// first.
varTable       *funcTable_get_args(funcTable * vt, int i);

// Returns the declaration line of the variable stored at the given index.
// No check is made by this function, so make sure that the index is valid
// first.
varTable       *funcTable_get_vt(funcTable * vt, int i);

// Returns the variable type stored at the given index.
// No check is made by this function, so make sure that the index is valid
// first.
Type            funcTable_get_type(funcTable * vt, int i);

// Prints the given table to stdout.
void            funcTable_print(funcTable * vt);

// Clears the allocated structure.
void            funcTable_free(funcTable * vt);

#endif				// TABLES_H
