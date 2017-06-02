/* File: parser.c */
/* Creation Date: 2016-12-24 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* The parser */

#include "core/compiler/parser/parser.h"

#include <string.h>

#include "err.h"

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/xmalloc.h"
#include "lib/xstack.h"

#include "core/compiler/ast/ast.h"
#include "core/compiler/compiler.h"
#include "core/compiler/parser/parse/parse_stmt/parse_stmt.h"
#include "core/compiler/token/token.h"
#include "core/object/object.h"

/********************/
/* Global variables */
/********************/

struct Parser parser;

/***********/
/* Methods */
/***********/

struct Token const* parser_check(const enum TokenType type)
{
  /* Retreive the current token */
  struct Token const* tok = parser_get();

  /* If current token type is not the given */
  if (tok->type != type) {
    /* Set the appropriate error message */
    compiler_err_setf(parser_get_ln(),
                      "expected \"%s\", got \"%s\"",
                      tt2str(type),
                      tok2str(tok));
    /* Fail */
    return NULL;
  }

  /* Succeed */
  return tok;
}

struct Token const* parser_eat(const enum TokenType type)
{
  /* Check that the current token is a token of a given type */
  struct Token const* res = parser_check(type);

  /* If not, fail */
  if (!res) return NULL;

  /* Otherwise, skip */
  parser_skip();

  /* Succeed */
  return res;
}

bool parser_eat_sep_or_end(void)
{
  /* Read the token */
  struct Token const* tok = parser_get();

  /* If token is not a separator or end token */
  if (!tok_is_sep(tok) && !tok_is_invalid(tok)) {
    /* Set the appropriate error message */
    compiler_err_setf(
      parser_get_ln(), "separator or EOF expected, got \"%s\"", tok2str(tok));
    /* Fail */
    return false;
  }

  /* Skip it */
  parser_skip();

  /* Succeed */
  return true;
}

NoRet parser_skip_newlines(void)
{
  struct Token const* tok;
  while ((tok = parser_get())->type == TOK_SEP_NEWLINE) {
    parser_incr_ln();
    parser_skip();
  }
}

/**************/
/* Benchmarks */
/**************/

#if BENCH
NoRet bench_parser(void)
{
}
#endif /* BENCH */

/*********/
/* Tests */
/*********/

#if TEST
#include <assert.h>

#include "core/compiler/lexer/lex/lex.h"
#include "core/compiler/parser/parse/parse.h"

#include "err.h"
#include "platform/path.h"
#include "platform/platform.h"
#include "platform/xfile.h"

static NoRet TEST_PARSE_CONDITION(const char const* name,
                                  const char const* path,
                                  const bool cond)
{
  err_reset();
  XFile file;
  char* file_contents;
  ByteSize file_contents_size;
  struct XVector* toks;
  struct AST* res = NULL;

  char* path_ = path_concat(PARSER_TESTS_RESOURCES_DIR, path);
  assert(xfile_open(&file, path_, XFM_R, false, false));
  xfree(path_);

  assert(xfile_load(&file, &file_contents, &file_contents_size));
  toks = lex(file_contents);
  assert(toks);
  res = parse(toks);

  if (cond)
    assert(err_is_ok());
  else
    assert(!(res || err_is_ok()));

  xvec_delete(toks);
  if (res) ast_delete(res);
  assert(xfile_close(&file));

  xfree(file_contents);
  err_reset();
  printf("\t[\"%s\"]: passed\n", name);
}

NoRet test_parser(void)
{
  TEST_PARSE_CONDITION("bad imports", "bad/imports.gshl", false);
  TEST_PARSE_CONDITION("good imports", "good/imports.gshl", true);

  TEST_PARSE_CONDITION(
    "bad compound expressions", "bad/expr_compound.gshl", false);
  TEST_PARSE_CONDITION(
    "good compound expressions", "good/expr_compound.gshl", true);

  TEST_PARSE_CONDITION(
    "bad variable definition statement", "bad/var_def.gshl", false);
  TEST_PARSE_CONDITION(
    "good variable definition statement", "good/var_def.gshl", true);

  TEST_PARSE_CONDITION(
    "bad const definition statement", "bad/const_def.gshl", false);
  TEST_PARSE_CONDITION(
    "good const definition statement", "good/const_def.gshl", true);

  TEST_PARSE_CONDITION("bad if statement", "bad/stmt_if.gshl", false);
  TEST_PARSE_CONDITION("good if statement", "good/stmt_if.gshl", true);

  TEST_PARSE_CONDITION("bad while statement", "bad/stmt_while.gshl", false);
  TEST_PARSE_CONDITION("good while statement", "good/stmt_while.gshl", true);

  TEST_PARSE_CONDITION("bad function definition", "bad/func_def.gshl", false);
  TEST_PARSE_CONDITION("good function definition", "good/func_def.gshl", true);
}

#endif /* TEST */
