/* File: flow_stmt.c */
/* Creation Date: 2017-01-19 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* parse_flow_stmt() function */

#include "core/compiler/parser/parse/parse_stmt/parse_flow_stmt/parse_flow_stmt.h"

#include "core/compiler/ast/ast.h"
#include "core/compiler/parser/parser.h"
#include "core/compiler/parser/parse/parse_stmt/parse_flow_stmt/parse_return_stmt.h"
#include "core/compiler/parser/parse/parse_stmt/parse_flow_stmt/parse_continue_stmt.h"
#include "core/compiler/parser/parse/parse_stmt/parse_flow_stmt/parse_break_stmt.h"
#include "core/compiler/parser/parse/parse_stmt/parse_flow_stmt/parse_if_stmt.h"

/* flow_stmt = return_stmt | continue_stmt | break_stmt; */
struct ASTNode* parse_flow_stmt(void)
{
    switch (parser_get()->type) {
        case TOK_KEYWORD_RETURN: return parse_return_stmt();
        case TOK_KEYWORD_CONTINUE: return parse_continue_stmt();
        case TOK_KEYWORD_BREAK: return parse_break_stmt();
        case TOK_KEYWORD_IF: return parse_if_stmt();
        default:
            parser_err_set_invalid_syntax();
            /* Fail */
            return NULL;
    }
}
