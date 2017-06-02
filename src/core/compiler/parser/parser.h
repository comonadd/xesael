/* File: parser.h */
/* Creation Date: 2016-12-24 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* The parser */
/* This file is the main file of the GSh parser. */
/* This defines all the things that other parser components */
/* probably would use */

#ifndef PARSER_H
#define PARSER_H

#include "err.h"

#include "lib/types.h"
#include "lib/xvec_stream.h"

#include "core/compiler/ast/ast.h"
#include "core/compiler/compiler.h"

/**
   @stream - stream of tokens
   @ln - current line number
   @ast - output
**/
struct Parser {
  struct XVectorStream stream;
  LineNumber ln;
  struct AST* ast;
};

/********************/
/* Global variables */
/********************/

extern struct Parser parser;

/***********/
/* Methods */
/***********/

#define parser_err_set_invalid_syntax() \
  compiler_err_set(parser_get_ln(), "invalid syntax")

#define parser_read() \
  CAST(xvec_stream_read(&parser.stream), struct Token const*)
#define parser_skip() xvec_stream_skip(&parser.stream)
#define parser_get() CAST(xvec_stream_get(&parser.stream), struct Token const*)
#define parser_lookahead() \
  CAST(xvec_stream_lookahead(&parser.stream), struct Token const*)
#define parser_lookback() \
  CAST(xvec_stream_lookback(&parser.stream), struct Token const*)

#define parser_incr_ln() (++parser.ln)
#define parser_get_ln() parser.ln
#define parser_lookahead_eq(___t) (parser_lookahead()->type == (___t))
#define parser_lookback_eq(___t) (parser_lookback()->type == (___t))
#define parser_eq(___t) (parser_get()->type == (___t))
#define parser_neq(___t) (parser_get()->type != (___t))

#define parser_stream_init(___toks)                                   \
  do {                                                                \
    xvec_stream_init(&parser.stream, (___toks), &RTOKS[TOK_INVALID]); \
  } while (0)
#define parser_init(___toks)       \
  do {                             \
    parser_stream_init((___toks)); \
    parser.ln  = 1;                \
    parser.ast = ast_new();        \
    err_reset();                   \
  } while (0)

/**
   $ Description:
   #   This function checks that
   #   thecurrent token is a token of a given type
**/
struct Token const* parser_check(const enum TokenType type);

/**
   $ Description:
   #   This function checks that
   #   the current token is a token of a given type
   #   and if so, skip it
**/
struct Token const* parser_eat(const enum TokenType type);

/**
   $ Description:
   #
**/
bool parser_eat_sep_or_end(void);

NoRet parser_skip_newlines(void);

/**************/
/* Benchmarks */
/**************/

#if BENCH
NoRet bench_parser(void);
#endif /* BENCH */

/*********/
/* Tests */
/*********/

#if TEST
NoRet test_parser(void);
#endif /* TEST */

#endif /* PARSER_H */
