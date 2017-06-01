/* File: parse_loop_stmt.c */
/* Creation date: 2017-02-04 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* parse_loop_stmt() function */

#include "core/compiler/parser/parse/parse_stmt/parse_loop_stmt/parse_loop_stmt.h"

#include "core/compiler/ast/ast_node.h"
#include "core/compiler/parser/parser.h"
#include "core/compiler/parser/parse/parse_stmt/parse_loop_stmt/parse_generic_loop_stmt.h"
#include "core/compiler/parser/parse/parse_stmt/parse_loop_stmt/parse_for_loop_stmt.h"
#include "core/compiler/parser/parse/parse_stmt/parse_loop_stmt/parse_while_loop_stmt.h"

/* loop_stmt = generic_loop_stmt */
/*           | for_loop_stmt     */
/*           | while_loop_stmt;  */
struct ASTNode* parse_loop_stmt(void)
{
    /* Retreive the current token */
    struct Token const* tok = parser_get();

    /* Do some parsing depending on the token type */
    switch (tok->type) {
        case TOK_KEYWORD_LOOP: return parse_generic_loop_stmt();
        case TOK_KEYWORD_FOR: return parse_for_loop_stmt();
        case TOK_KEYWORD_WHILE: return parse_while_loop_stmt();
        default:
            IMPOSSIBLE();
            /* Fail */
            return NULL;
    }
}
