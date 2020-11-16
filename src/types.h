
#ifndef TYPES_H
#define TYPES_H

typedef enum {
    VOID_TYPE,
    INT_TYPE,
    FLOAT_TYPE,
    CHAR_TYPE
} Type;

const char     *get_text(Type type);

#endif				// TYPES_H
