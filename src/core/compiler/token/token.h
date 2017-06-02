/* File: token.h */
/* Creation date: 11.12.2016 */
/* Author: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* Token structure definition and helpers for working with it */

#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/xvec.h"

#define RTOK_FIRST 0

typedef uint64 TokenPrecedence;

/* NOTE: Order reserved: */
/*   TOK_POS should be the first unary operator token type */
/*   TOK_IN should be the last unary operator token type */
/*   TOK_ASSIGN should be the first assign binary token type */
/*   TOK_ASSIGN_RS should be the last assign binary token type */
enum TokenType {
  /* Invalid token */
  TOK_INVALID = RTOK_FIRST,

  /* Separators */
  TOK_SEP_NEWLINE,
  TOK_SEP_SEMI,
  TOK_SEP_COLON,
  TOK_SEP_COMMA,
  TOK_SEP_LBRACKET,
  TOK_SEP_RBRACKET,
  TOK_SEP_LPAREN,
  TOK_SEP_RPAREN,
  TOK_SEP_LBRACE,
  TOK_SEP_RBRACE,

  /* Unary prefix oparators */
  TOK_NEG,
  TOK_LNOT,
  TOK_BNOT,
  TOK_LEN,

  /* Unary postfix operators */
  TOK_INCR,
  TOK_DECR,

  /* Binary arithmetic operators */
  TOK_ADD,
  TOK_SUB,
  TOK_MUL,
  TOK_DIV,
  TOK_MOD,
  TOK_POW,
  TOK_BAND,
  TOK_BOR,
  TOK_BXOR,
  TOK_LS,
  TOK_RS,

  /* Binary boolean operators */
  TOK_LAND,
  TOK_LOR,

  /* Binary relational operators */
  TOK_EQ,
  TOK_NEQ,
  TOK_LT,
  TOK_GT,
  TOK_LTE,
  TOK_GTE,
  TOK_IN,

  /* Assign operator */
  TOK_ASSIGN,

  /* Arithmetic assign operator shorthands */
  TOK_ASSIGN_ADD,
  TOK_ASSIGN_SUB,
  TOK_ASSIGN_MUL,
  TOK_ASSIGN_DIV,
  TOK_ASSIGN_MOD,
  TOK_ASSIGN_POW,
  TOK_ASSIGN_BAND,
  TOK_ASSIGN_BOR,
  TOK_ASSIGN_BXOR,
  TOK_ASSIGN_LS,
  TOK_ASSIGN_RS,

  /* Boolean assign operator shorthands */
  TOK_ASSIGN_LAND,
  TOK_ASSIGN_LOR,

  /* "define" keywords */
  TOK_KEYWORD_VAR,
  TOK_KEYWORD_CONST,
  TOK_KEYWORD_ENUM,
  TOK_KEYWORD_FUNC,

  /* "flow" keywords */
  TOK_KEYWORD_CONTINUE,
  TOK_KEYWORD_BREAK,
  TOK_KEYWORD_RETURN,
  TOK_KEYWORD_IF,
  TOK_KEYWORD_ELIF,
  TOK_KEYWORD_ELSE,

  /* "loop" keywords */
  TOK_KEYWORD_LOOP,
  TOK_KEYWORD_FOR,
  TOK_KEYWORD_WHILE,

  /* Keywords */
  TOK_KEYWORD_TRUE,
  TOK_KEYWORD_FALSE,
  TOK_KEYWORD_IMPORT,

  /* Not reserved */
  TOK_INUMLIT,
  TOK_DNUMLIT,
  TOK_STRLIT,
  TOK_ID,

  /* Just the count of tokens */
  TOKS_COUNT
};

#define RTOK_LAST TOK_KEYWORD_IMPORT
#define RTOKS_COUNT RTOK_LAST - RTOK_FIRST + 1

#define TOK_SEP_FIRST TOK_SEP_NEWLINE
#define TOK_SEP_LAST TOK_SEP_RBRACE

#define TOK_UNOP_FIRST TOK_NEG
#define TOK_UNOP_LAST TOK_DECR

#define TOK_PREFIX_UNOP_FIRST TOK_NEG
#define TOK_PREFIX_UNOP_LAST TOK_IN

#define TOK_POSTFIX_UNOP_FIRST TOK_INCR
#define TOK_POSTFIX_UNOP_LAST TOK_DECR

#define TOK_BINOP_FIRST TOK_ADD
#define TOK_BINOP_LAST TOK_ASSIGN_RS

#define TOK_ASSIGN_FIRST TOK_ASSIGN
#define TOK_ASSIGN_LAST TOK_ASSIGN_RS

#define TOK_ASSIGN_SHORTHAND_FIRST TOK_ASSIGN_ADD
#define TOK_ASSIGN_SHORTHAND_LAST TOK_ASSIGN_RS

#define TOK_KEYWORD_FIRST TOK_KEYWORD_VAR
#define TOK_KEYWORD_LAST TOK_KEYWORD_IMPORT

struct Token {
  struct Object* val;
  enum TokenType type;
};

extern const struct Token RTOKS[RTOKS_COUNT];
extern const char* TOKS_STR[TOKS_COUNT];

/****************/
/* "Is" helpers */
/****************/

#define tt_is_sep(__type) \
  (((__type) >= TOK_SEP_FIRST) && ((__type) <= TOK_SEP_LAST))
#define tok_is_sep(tok) tt_is_sep(tok->type)

#define tt_is_unop(__type) \
  (((__type) >= TOK_UNOP_FIRST) && ((__type) <= TOK_UNOP_LAST))
#define tok_is_unop(tok) tt_is_unop(tok->type)

#define tt_is_prefix_unop(__type) \
  (((__type) >= TOK_PREFIX_UNOP_FIRST) && ((__type) <= TOK_PREFIX_UNOP_LAST))
#define tok_is_prefix_unop(tok) tt_is_prefix_unop((tok)->type)

#define tt_is_postfix_unop(__type) \
  (((__type) >= TOK_POSTFIX_UNOP_FIRST) && ((__type) <= TOK_POSTFIX_UNOP_LAST))
#define tok_is_postfix_unop(tok) tt_is_postfix_unop((tok)->type)

#define tt_is_binop(___type) \
  (((___type) >= TOK_BINOP_FIRST) && ((___type) <= TOK_BINOP_LAST))
#define tok_is_binop(___tok) tt_is_binop((___tok)->type)

#define tt_is_assign_shorthand(__type)       \
  ((__type >= TOK_ASSIGN_SHORTHAND_FIRST) && \
   (__type <= TOK_ASSIGN_SHORTHAND_LAST))
#define tok_is_assign_shorthand(tok) tt_is_assign_shorthand(tok->type)

#define tt_is_assign(__type) \
  (((__type) >= TOK_ASSIGN_FIRST) && ((__type) <= TOK_ASSIGN_LAST))
#define tok_is_assign(tok) tt_is_assign(tok->type)

#define tt_is_keyword(__type) \
  (((__type) >= TOK_KEYWORD_FIRST) && ((__type) <= TOK_KEYWORD_LAST))
#define tok_is_keyword(tok) tt_is_keyword(tok - type)

#define tok_is_invalid(___tok) ((___tok)->type == TOK_INVALID)
#define tok_is_op(___tok) (tok_is_unop((___tok)) || tok_is_binop(___(tok)))
#define tok_is_reserved(___tok) ((___tok)->type <= RTOK_LAST)
#define tok_is_id(___tok) !tok_is_reserved((___tok))

bool tt_is_left_assoc(const enum TokenType type);
#define tok_is_left_assoc(___tok) (tt_is_left_assoc((___tok)->type))

/**********************/
/* Conversion helpers */
/**********************/

#define tt2str(___type) TOKS_STR[(___type)]
#define tok2str(__tok) tt2str(CAST((__tok), struct Token*)->type)

/***********/
/* Helpers */
/***********/

#define print_tok(___tok) fputs(tok2str((___tok)), stdout)
#define tok_assert_type(___tok, ___tok_type) \
  DASSERT((___tok)->type == (___tok_type), "")
NoRet tok_free_f(void* tok);
bool tok_is_stmt_sep(struct Token const* tok);
bool tok_can_start_def_stmt(struct Token const* tok);
bool tok_can_start_flow_stmt(struct Token const* tok);
bool tok_can_start_loop_stmt(struct Token const* tok);

/*****************/
/* Debug helpers */
/*****************/

#if DEBUG
NoRet tok_xvec_print(const struct XVector* vec);
#endif /* DEBUG */

/****************/
/* Constructors */
/****************/

struct Token* tok_strlit_new(const char* val);
struct Token* tok_inumlit_new(const char* val);
struct Token* tok_dnumlit_new(const char* val);
struct Token* tok_id_new(const char* val);

#endif /* TOKEN_H */
