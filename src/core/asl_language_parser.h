/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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

#ifndef YY_YY_ASL_LANGUAGE_PARSER_H_INCLUDED
# define YY_YY_ASL_LANGUAGE_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    T_LITERAL_DIGIT = 258,
    T_LITERAL_DOUBLE = 259,
    T_FUNCTION = 260,
    T_GLOBAL = 261,
    T_FOR = 262,
    T_IF = 263,
    T_ELSE = 264,
    T_RETURN = 265,
    T_INCLUDE = 266,
    T_CONTINUE = 267,
    T_BREAK = 268,
    T_WHILE = 269,
    T_DO = 270,
    T_TRUE = 271,
    T_FALSE = 272,
    T_NULL = 273,
    T_PLUS = 274,
    T_SUB = 275,
    T_MUL = 276,
    T_DIV = 277,
    T_ASSIGN = 278,
    T_EQ = 279,
    T_NEQ = 280,
    T_GE = 281,
    T_GT = 282,
    T_LE = 283,
    T_LT = 284,
    T_LP = 285,
    T_RP = 286,
    T_LBRACE = 287,
    T_RBRACE = 288,
    T_SEMICOLON = 289,
    T_COMMA = 290,
    T_IDENTIFIER = 291,
    T_LITERAL_STRING = 292,
    T_VARIABLE = 293
  };
#endif
/* Tokens.  */
#define T_LITERAL_DIGIT 258
#define T_LITERAL_DOUBLE 259
#define T_FUNCTION 260
#define T_GLOBAL 261
#define T_FOR 262
#define T_IF 263
#define T_ELSE 264
#define T_RETURN 265
#define T_INCLUDE 266
#define T_CONTINUE 267
#define T_BREAK 268
#define T_WHILE 269
#define T_DO 270
#define T_TRUE 271
#define T_FALSE 272
#define T_NULL 273
#define T_PLUS 274
#define T_SUB 275
#define T_MUL 276
#define T_DIV 277
#define T_ASSIGN 278
#define T_EQ 279
#define T_NEQ 280
#define T_GE 281
#define T_GT 282
#define T_LE 283
#define T_LT 284
#define T_LP 285
#define T_RP 286
#define T_LBRACE 287
#define T_RBRACE 288
#define T_SEMICOLON 289
#define T_COMMA 290
#define T_IDENTIFIER 291
#define T_LITERAL_STRING 292
#define T_VARIABLE 293

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 9 "asl_language_parser.y" /* yacc.c:1909  */

    char *variable;
	char *identifier;
	int digit;
	double db;
	exp_t *expression;
	exp_list_t *exp_list;
	statement_t *statement;
	statement_list_t *statement_list;
	func_t *function;
	func_param_list_t *func_param_list;
	func_param_t *func_param;

#line 144 "asl_language_parser.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_ASL_LANGUAGE_PARSER_H_INCLUDED  */
