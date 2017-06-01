/* File: parse_stmt_while.c */
/* Creation Date: 2017-01-30 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* parse_while_loop_stmt() function */

#include "core/compiler/parser/parse/parse_stmt/parse_loop_stmt/parse_while_loop_stmt.h"

#include "core/compiler/ast/ast_node.h"
#include "core/compiler/parser/parser.h"
#include "core/compiler/parser/parse/parse_expr/parse_expr.h"
#include "core/compiler/parser/parse/parse_stmt/parse_stmt.h"

/* while_stmt = WHILE_KEYWORD while_cond while_body; */
/* while_cond = LPAREN expr RPAREN; */
/* while_body = stmt; */
struct ASTNode* parse_while_loop_stmt(void)
{
    /* Obvious check */
    tok_assert_type(parser_get(), TOK_KEYWORD_WHILE);

    /* Save the line number that statement was defined on */
    const LineNumber ln = parser_get_ln();

    /* Skip 'while' token */
    parser_skip();

    /* Skip separators */
    parser_skip_newlines();

    /* Check that the opening paren '(' exists  */
    if (!parser_check(TOK_SEP_LPAREN)) return NULL;

    /* Parse the condition */
    struct ASTNode* cond_node = parse_expr();
    if (!cond_node) return NULL;

    /* Parse the body */
    struct ASTNode* body_node = parse_stmt();
    if (!body_node) {
        /* Delete the allocated node */
        ast_node_delete(cond_node);
        /* Fail */
        return NULL;
    }

    /* Create and return a new WHILE AST node */
    return mk_ast_node_while(ln, cond_node, body_node);
}
