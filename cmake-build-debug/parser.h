/* A Bison parser, made by GNU Bison 3.0.5.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_PARSER_H_INCLUDED
# define YY_YY_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 1 "/Users/zoe/Developer/hive/hive/parser/parser.y" /* yacc.c:1916  */
 
#include "../ast/Number/ast.h"
#include "../ast/BinOp/ast.h"
#include "../ast/Functions/ast.h"
#include "../ast/Variables/ast.h"
#include "../ast/ast.h"
#include "../globals.h"

#include "llvm/IR/Constants.h"

#line 55 "parser.h" /* yacc.c:1916  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TIDENTIFIER = 258,
    TINT = 259,
    TBINOP = 260,
    TOPENPAREN = 261,
    TCLOSEPAREN = 262,
    TOPENBRACE = 263,
    TCLOSEBRACE = 264,
    TFUNC = 265,
    TVAR = 266,
    TCAST = 267,
    TWDOUBLE = 268,
    TWINT = 269,
    TEQUALS = 270,
    TEXTERN = 271,
    TAND = 272,
    TOPENBRACKET = 273,
    TCLOSEBRACKET = 274,
    TSTAR = 275,
    TEND = 276,
    TCOLON = 277,
    TCOMMA = 278,
    arg = 279
  };
#endif
/* Tokens.  */
#define TIDENTIFIER 258
#define TINT 259
#define TBINOP 260
#define TOPENPAREN 261
#define TCLOSEPAREN 262
#define TOPENBRACE 263
#define TCLOSEBRACE 264
#define TFUNC 265
#define TVAR 266
#define TCAST 267
#define TWDOUBLE 268
#define TWINT 269
#define TEQUALS 270
#define TEXTERN 271
#define TAND 272
#define TOPENBRACKET 273
#define TCLOSEBRACKET 274
#define TSTAR 275
#define TEND 276
#define TCOLON 277
#define TCOMMA 278
#define arg 279

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 40 "/Users/zoe/Developer/hive/hive/parser/parser.y" /* yacc.c:1916  */

    std::string *string;
    NumberAST *number;
    BlockAST *base;
    BinOpAST *binOp;
    AST *any;
    FunctionAST *func;
    std::vector<std::pair<std::string, llvm::Type *>> *args;
    std::vector<Type *> *typeArgs;
    std::vector<AST *> *valueArgs;
    llvm::Type *type;

#line 128 "parser.h" /* yacc.c:1916  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_H_INCLUDED  */
