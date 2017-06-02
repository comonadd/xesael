/* File: parse_return_stmt.c */
/* Creation Date: 2017-01-30 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* parse_return_stmt() function */

#include "core/compiler/parser/parse/parse_stmt/parse_flow_stmt/parse_return_stmt.h"

#include "core/compiler/ast/ast_node.h"
#include "core/compiler/parser/parse/parse_expr/parse_expr.h"
#include "core/compiler/parser/parser.h"

/* return_stmt: RETURN_KEYWORD expr */
struct ASTNode* parse_return_stmt(void)
{
  /* Obvious check */
  tok_assert_type(parser_get(), TOK_KEYWORD_RETURN);

  /* Save the line number that statement was defined on */
  const LineNumber ln = parser_get_ln();

  /* Skip the "return" keyword */
  parser_skip();

  /* Parse the expression */
  struct ASTNode* expr = parse_expr();
  if (!expr) return NULL;

  /* Create and return a new RETURN AST node */
  return mk_ast_node_return(ln, expr);
}
