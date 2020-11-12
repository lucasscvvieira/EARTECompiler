
#include "types.h"

static const char *TYPE_STRING[] = {
    "void",
    "int",
    "float",
    "char"
};

const char     *
get_text(Type type)
{
    return TYPE_STRING[type];
}
