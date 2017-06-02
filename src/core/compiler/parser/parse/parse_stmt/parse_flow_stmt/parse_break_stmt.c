/* File: parse_break_stmt.c */
/* Creation Date: 2017-01-30 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* parse_break_stmt() function */

#include "core/compiler/parser/parse/parse_stmt/parse_flow_stmt/parse_break_stmt.h"

#include "core/compiler/ast/ast_node.h"
#include "core/compiler/compiler.h"
#include "core/compiler/parser/parser.h"

/* break_stmt = BREAK_KEYWORD; */
struct ASTNode* parse_break_stmt(void)
{
  /* Obvious check */
  tok_assert_type(parser_get(), TOK_KEYWORD_BREAK);

  /* Not implemented */
  parser_err_set_invalid_syntax();
  return NULL;
}
