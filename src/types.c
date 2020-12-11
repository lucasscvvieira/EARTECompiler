
#include "types.h"

static const char *TYPE_STRING[] = {
	"void",
	"int",
	"int[]",
	"float",
	"float[]",
	"char",
	"char[]"
};

const char *get_text(Type type)
{
	return TYPE_STRING[type];
}

static const Unif plus[7][7] = {
    // *INDENT-OFF*
                        //VOID_TYPE                 INT_TYPE                    INT_ARRAY_TYPE          FLOAT_TYPE                 FLOAT_ARRAY_TYPE         CHAR_TYPE                   CHAR_ARRAY_TYPE
    /*VOID_TYPE*/       { {NO_TYPE, NONE, NONE},    {NO_TYPE, NONE, NONE},      {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},     {NO_TYPE, NONE, NONE},   {NO_TYPE, NONE, NONE},      {NO_TYPE, NONE, NONE} },
    /*INT_TYPE*/        { {NO_TYPE, NONE, NONE},    {INT_TYPE, NONE, NONE},     {NO_TYPE, NONE, NONE},  {FLOAT_TYPE, I2F, NONE},   {NO_TYPE, NONE, NONE},   {INT_TYPE, NONE, C2I},      {NO_TYPE, NONE, NONE} },
    /*INT_ARRAY_TYPE*/  { {NO_TYPE, NONE, NONE},    {NO_TYPE, NONE, NONE},      {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},     {NO_TYPE, NONE, NONE},   {NO_TYPE, NONE, NONE},      {NO_TYPE, NONE, NONE} },
    /*FLOAT_TYPE*/      { {NO_TYPE, NONE, NONE},    {FLOAT_TYPE, NONE, I2F},    {NO_TYPE, NONE, NONE},  {FLOAT_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},   {NO_TYPE, NONE, NONE},      {NO_TYPE, NONE, NONE} },
    /*FLOAT_ARRAY_TYPE*/{ {NO_TYPE, NONE, NONE},    {NO_TYPE, NONE, NONE},      {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},     {NO_TYPE, NONE, NONE},   {NO_TYPE, NONE, NONE},      {NO_TYPE, NONE, NONE} },
    /*CHAR_TYPE*/       { {NO_TYPE, NONE, NONE},    {CHAR_TYPE, NONE, I2C},     {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},     {NO_TYPE, NONE, NONE},   {CHAR_TYPE, NONE, NONE},    {NO_TYPE, NONE, NONE} },
    /*CHAR_ARRAY_TYPE*/ { {NO_TYPE, NONE, NONE},    {NO_TYPE, NONE, NONE},      {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},     {NO_TYPE, NONE, NONE},   {NO_TYPE, NONE, NONE},      {NO_TYPE, NONE, NONE} }
    // *INDENT-ON*
};

Unif unify_plus(Type lt, Type rt)
{
	return plus[lt][rt];
}

static const Unif other[7][7] = {
    // *INDENT-OFF*
                        //VOID_TYPE                 INT_TYPE                    INT_ARRAY_TYPE          FLOAT_TYPE                  FLOAT_ARRAY_TYPE        CHAR_TYPE                   CHAR_ARRAY_TYPE
    /*VOID_TYPE*/       { {NO_TYPE, NONE, NONE},    {NO_TYPE, NONE, NONE},      {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},      {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},      {NO_TYPE, NONE, NONE} },
    /*INT_TYPE*/        { {NO_TYPE, NONE, NONE},    {INT_TYPE, NONE, NONE},     {NO_TYPE, NONE, NONE},  {FLOAT_TYPE, I2F, NONE},    {NO_TYPE, NONE, NONE},  {INT_TYPE, NONE, C2I},      {NO_TYPE, NONE, NONE} },
    /*INT_ARRAY_TYPE*/  { {NO_TYPE, NONE, NONE},    {NO_TYPE, NONE, NONE},      {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},      {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},      {NO_TYPE, NONE, NONE} },
    /*FLOAT_TYPE*/      { {NO_TYPE, NONE, NONE},    {FLOAT_TYPE, NONE, I2F},    {NO_TYPE, NONE, NONE},  {FLOAT_TYPE, NONE, NONE},   {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},      {NO_TYPE, NONE, NONE} },
    /*FLOAT_ARRAY_TYPE*/{ {NO_TYPE, NONE, NONE},    {NO_TYPE, NONE, NONE},      {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},      {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},      {NO_TYPE, NONE, NONE} },
    /*CHAR_TYPE*/       { {NO_TYPE, NONE, NONE},    {CHAR_TYPE, NONE, I2C},     {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},      {NO_TYPE, NONE, NONE},  {CHAR_TYPE, NONE, NONE},    {NO_TYPE, NONE, NONE} },
    /*CHAR_ARRAY_TYPE*/ { {NO_TYPE, NONE, NONE},    {NO_TYPE, NONE, NONE},      {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},      {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},      {NO_TYPE, NONE, NONE} }
    // *INDENT-ON*
};

Unif unify_other_arith(Type lt, Type rt)
{
	return other[lt][rt];
}

static const Unif comp[7][7] = {
    // *INDENT-OFF*
                        //VOID_TYPE                 INT_TYPE                INT_ARRAY_TYPE          FLOAT_TYPE              FLOAT_ARRAY_TYPE        CHAR_TYPE               CHAR_ARRAY_TYPE
    /*VOID_TYPE*/       { {NO_TYPE, NONE, NONE},    {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE} },
    /*INT_TYPE*/        { {NO_TYPE, NONE, NONE},    {INT_TYPE, NONE, NONE}, {NO_TYPE, NONE, NONE},  {INT_TYPE, NONE, NONE}, {NO_TYPE, NONE, NONE},  {INT_TYPE, NONE, NONE}, {NO_TYPE, NONE, NONE} },
    /*INT_ARRAY_TYPE*/  { {NO_TYPE, NONE, NONE},    {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE} },
    /*FLOAT_TYPE*/      { {NO_TYPE, NONE, NONE},    {INT_TYPE, NONE, NONE}, {NO_TYPE, NONE, NONE},  {INT_TYPE, NONE, NONE}, {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE} },
    /*FLOAT_ARRAY_TYPE*/{ {NO_TYPE, NONE, NONE},    {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE} },
    /*CHAR_TYPE*/       { {NO_TYPE, NONE, NONE},    {INT_TYPE, NONE, NONE}, {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {INT_TYPE, NONE, NONE}, {NO_TYPE, NONE, NONE} },
    /*CHAR_ARRAY_TYPE*/ { {NO_TYPE, NONE, NONE},    {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE} }
    // *INDENT-ON*
};

Unif unify_comp(Type lt, Type rt)
{
	return comp[lt][rt];
}

static const Unif logic[7][7] = {
    // *INDENT-OFF*
                        //VOID_TYPE                 INT_TYPE                INT_ARRAY_TYPE          FLOAT_TYPE              FLOAT_ARRAY_TYPE        CHAR_TYPE               CHAR_ARRAY_TYPE
    /*VOID_TYPE*/       { {NO_TYPE, NONE, NONE},    {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE} },
    /*INT_TYPE*/        { {NO_TYPE, NONE, NONE},    {INT_TYPE, NONE, NONE}, {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {INT_TYPE, NONE, NONE}, {NO_TYPE, NONE, NONE} },
    /*INT_ARRAY_TYPE*/  { {NO_TYPE, NONE, NONE},    {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE} },
    /*FLOAT_TYPE*/      { {NO_TYPE, NONE, NONE},    {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE} },
    /*FLOAT_ARRAY_TYPE*/{ {NO_TYPE, NONE, NONE},    {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE} },
    /*CHAR_TYPE*/       { {NO_TYPE, NONE, NONE},    {INT_TYPE, NONE, NONE}, {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {INT_TYPE, NONE, NONE}, {NO_TYPE, NONE, NONE} },
    /*CHAR_ARRAY_TYPE*/ { {NO_TYPE, NONE, NONE},    {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE},  {NO_TYPE, NONE, NONE} }
    // *INDENT-ON*
};

Unif unify_logic(Type lt, Type rt)
{
	return logic[lt][rt];
}
