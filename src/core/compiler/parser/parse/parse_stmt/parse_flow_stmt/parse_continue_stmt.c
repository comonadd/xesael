/* File: parse_continue_stmt.c */
/* Creation Date: 2017-01-30 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* parse_continue_stmt() function */

#include "core/compiler/parser/parse/parse_stmt/parse_flow_stmt/parse_continue_stmt.h"

#include "core/compiler/token/token.h"
#include "core/compiler/ast/ast_node.h"
#include "core/compiler/parser/parser.h"

/* continue_stmt = CONTINUE_KEYWORD */
/* TODO: Implement */
struct ASTNode* parse_continue_stmt(void)
{
    /* Obvious check */
    tok_assert_type(parser_get(), TOK_KEYWORD_CONTINUE);

    /* Not implemented yet */
    parser_err_set_invalid_syntax();
    return NULL;
}
