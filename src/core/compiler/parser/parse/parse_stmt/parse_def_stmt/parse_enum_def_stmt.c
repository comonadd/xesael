/* File: parse_enum_def_stmt.c */
/* Creation Date: 2017-01-30 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* parse_stmt_enumdef() function */

#include "core/compiler/parser/parse/parse_stmt/parse_def_stmt/parse_enum_def_stmt.h"

#include "core/compiler/token/token.h"
#include "core/compiler/ast/ast_node.h"
#include "core/compiler/parser/parser.h"
#include "core/compiler/parser/parse/parse_stmt/parse_def_stmt/parse_initializator.h"

/* enumdef_stmt = ENUM_KEYWORD identifier LBRACE {identifier [EQUALS literal]} RBRACE; */
/* TODO: Implement this */
struct ASTNode* parse_stmt_enumdef(void)
{
    /* Obvious check */
    tok_assert_type(parser_get(), TOK_KEYWORD_ENUM);
    return NULL;
}
