/* File: parse_initializator.c */
/* Creation Date: 2017-01-30 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */

#include "core/compiler/parser/parse/parse_stmt/parse_def_stmt/parse_initializator.h"

#include "core/compiler/ast/ast_node.h"
#include "core/compiler/parser/parse/parse_expr/parse_expr.h"

struct ASTNode* parse_initializator(void)
{
  return parse_expr();
}
