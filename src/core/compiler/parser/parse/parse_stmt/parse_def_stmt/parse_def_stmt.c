/* File: parse_def_stmt.c */
/* Creation Date: 2017-01-30 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* parse_def_stmt() function */

#include "core/compiler/parser/parse/parse_stmt/parse_def_stmt/parse_def_stmt.h"

#include "core/compiler/ast/ast_node.h"
#include "core/compiler/parser/parse/parse_stmt/parse_def_stmt//parse_const_def_stmt.h"
#include "core/compiler/parser/parse/parse_stmt/parse_def_stmt//parse_enum_def_stmt.h"
#include "core/compiler/parser/parse/parse_stmt/parse_def_stmt//parse_func_def_stmt.h"
#include "core/compiler/parser/parse/parse_stmt/parse_def_stmt//parse_var_def_stmt.h"
#include "core/compiler/parser/parser.h"

/* def_stmt = vardef_stmt | */
/*            constdef_stmt | */
/*            enumdef_stmt | */
/*            funcdef_stmt; */
struct ASTNode* parse_def_stmt(void)
{
  switch (parser_get()->type) {
    case TOK_KEYWORD_VAR: return parse_var_def_stmt();
    case TOK_KEYWORD_CONST: return parse_const_def_stmt();
    case TOK_KEYWORD_ENUM: return parse_stmt_enumdef();
    case TOK_KEYWORD_FUNC: return parse_func_def_stmt();
    default:
      IMPOSSIBLE();
      /* Fail */
      return NULL;
  }
}
