/* File: parse_var_def_stmt.c */
/* Creation Date: 2017-01-30 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* parse_var_def_stmt() function */

#include "core/compiler/parser/parse/parse_stmt/parse_def_stmt/parse_var_def_stmt.h"

#include "core/compiler/ast/ast_node.h"
#include "core/compiler/parser/parse/parse_stmt/parse_def_stmt/parse_initializator.h"
#include "core/compiler/parser/parser.h"
#include "core/compiler/token/token.h"

/* vardef_stmt = VAR_KEYWORD identifier [EQUALS initializator]; */
struct ASTNode* parse_var_def_stmt(void)
{
  /* Obvious check */
  tok_assert_type(parser_get(), TOK_KEYWORD_VAR);

  /* Save the line number that statement was defined on */
  const LineNumber ln = parser_get_ln();

  /* Skip 'var' keyword token */
  parser_skip();

  /* Retreive the variable name */
  struct Token const* name_tok = parser_eat(TOK_ID);
  if (!name_tok) return NULL;
  struct Object* name_obj       = name_tok->val;
  struct ASTNode* initializator = NULL;

  /* If we have an end of expression (separator) */
  struct Token const* next_tok = parser_get();
  if (tok_is_sep(next_tok) || tok_is_invalid(next_tok)) {
    /* Skip the separator */
    parser_skip();
    /* Done */
    goto done;
  }

  if (!parser_eat(TOK_ASSIGN)) return NULL;

  /* Parse the initializator */
  initializator = parse_initializator();
  if (!initializator) return NULL;

done:
  /* Succeed */
  return mk_ast_node_var_def(ln, name_obj, initializator);
}
