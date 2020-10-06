/* A Bison parser, made by GNU Bison 2.1.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     ID = 258,
     PAREN_ID = 259,
     BRACKET_ID = 260,
     DBL_BRACKET_ID = 261,
     CURLY_ID = 262,
     ANGLE_ID = 263,
     BRACKET_ID_LIST = 264,
     NUM = 265,
     OPTS_STR = 266,
     SPECIAL = 267,
     TICK = 268,
     THEN = 269,
     DEF = 270,
     EMPTY_ANGLE = 271,
     DOTS = 272,
     LINE = 273,
     CURVE = 274,
     POLYGON = 275,
     REPEAT = 276,
     SWEEP = 277,
     PUT = 278,
     TRANSLATE = 279,
     ROTATE = 280,
     SCALE = 281,
     PROJECT = 282,
     PERSPECTIVE = 283,
     VIEW = 284,
     SQRT = 285,
     SIN = 286,
     COS = 287,
     ATAN2 = 288,
     UNIT = 289,
     INVERSE = 290,
     GLOBAL = 291,
     SET = 292,
     PICTUREBOX = 293,
     FRAME = 294,
     CAMERA = 295,
     LANGUAGE = 296,
     PSTRICKS = 297,
     TIKZ = 298,
     LaTeX = 299,
     ConTeXt = 300,
     NEG = 301
   };
#endif
/* Tokens.  */
#define ID 258
#define PAREN_ID 259
#define BRACKET_ID 260
#define DBL_BRACKET_ID 261
#define CURLY_ID 262
#define ANGLE_ID 263
#define BRACKET_ID_LIST 264
#define NUM 265
#define OPTS_STR 266
#define SPECIAL 267
#define TICK 268
#define THEN 269
#define DEF 270
#define EMPTY_ANGLE 271
#define DOTS 272
#define LINE 273
#define CURVE 274
#define POLYGON 275
#define REPEAT 276
#define SWEEP 277
#define PUT 278
#define TRANSLATE 279
#define ROTATE 280
#define SCALE 281
#define PROJECT 282
#define PERSPECTIVE 283
#define VIEW 284
#define SQRT 285
#define SIN 286
#define COS 287
#define ATAN2 288
#define UNIT 289
#define INVERSE 290
#define GLOBAL 291
#define SET 292
#define PICTUREBOX 293
#define FRAME 294
#define CAMERA 295
#define LANGUAGE 296
#define PSTRICKS 297
#define TIKZ 298
#define LaTeX 299
#define ConTeXt 300
#define NEG 301




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 58 "sketch.y"
typedef union YYSTYPE {
  char *str;
  FLOAT flt;
  POINT_3D pt;
  VECTOR_3D vec;
  TRANSFORM xf;
  EXPR_VAL exv;
  SYMBOL_NAME name;
  SYMBOL_NAME_NODE *name_list;
  OBJECT *obj;
  OPTS *opts;
  int bool;
  int index;
} YYSTYPE;
/* Line 1447 of yacc.c.  */
#line 145 "y.tab.h"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



