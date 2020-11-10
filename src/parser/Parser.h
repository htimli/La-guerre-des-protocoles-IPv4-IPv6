/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

#ifndef YY_YY_SRC_PARSER_PARSER_H_INCLUDED
# define YY_YY_SRC_PARSER_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 29 "src/parser/Parser.y" /* yacc.c:1909  */

  typedef void* yyscan_t;
  enum stateType {None,Init,Final,InitFinal};
  typedef struct {
      enum stateType automataInfo;
      char* color;
  } stateInformation;

#line 53 "src/parser/Parser.h" /* yacc.c:1909  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    T_LPAREN = 258,
    T_RPAREN = 259,
    T_COMMA = 260,
    T_COLON = 261,
    T_SEMI = 262,
    T_AT = 263,
    T_LBRACKET = 264,
    T_RBRACKET = 265,
    T_LBRACE = 266,
    T_RBRACE = 267,
    T_STRING = 268,
    T_EQ = 269,
    T_DIGRAPH = 270,
    T_EDGE = 271,
    T_DEDGE = 272,
    T_UEDGE = 273,
    T_GRAPH = 274,
    T_ID = 275,
    T_NODE = 276,
    T_STRICT = 277,
    T_SUBGRAPH = 278
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 49 "src/parser/Parser.y" /* yacc.c:1909  */

    char* name;
    stateInformation stateInfo;

#line 94 "src/parser/Parser.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int yyparse (GraphList *graph, yyscan_t scanner);

#endif /* !YY_YY_SRC_PARSER_PARSER_H_INCLUDED  */
