/* File: parse_if_stmt.c */
/* Creation Date: 2017-01-30 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */

#include "core/compiler/parser/parse/parse_stmt/parse_flow_stmt/parse_if_stmt.h"

#include "err.h"

#include "lib/types.h"

#include "core/compiler/ast/ast_node.h"
#include "core/compiler/parser/parser.h"
#include "core/compiler/parser/parse/parse_expr/parse_expr.h"
#include "core/compiler/parser/parse/parse_stmt/parse_stmt.h"

/* if_cond = LPAREN expr RPAREN; */
static struct ASTNode* __parse_cond(void)
{
    if (!parser_check(TOK_SEP_LPAREN)) return NULL;
    struct ASTNode* node = parse_expr();
    if (!node) return NULL;
    DASSERT(parser_lookback_eq(TOK_SEP_RPAREN), "");
    return node;
}

/* if_body = stmt; */
static struct ASTNode* __parse_body(void)
{
    return parse_stmt();
}

static bool __parse_elif_pairs(struct XVector* stmts)
{
    /* While there are ELIF_KEYWORD token */
    while (parser_eq(TOK_KEYWORD_ELIF)) {
        /* Skip ELIF_KEYWORD token */
        parser_skip();

        /* Parse the condition */
        struct ASTNode* cond;
        cond = __parse_cond();
        if (!cond) return false;

        /* Parse the body */
        struct ASTNode* body;
        body = __parse_body();
        if (!body) return false;

        /* Append the condition and body */
        xvec_append(stmts, cond);
        xvec_append(stmts, body);
    }

    /* Succeed */
    return true;
}

/* if_stmt = IF_KEYWORD if_cond if_body {[ELIF_KEYWORD if_cond if_body]} [ELSE_KEYWORD if_body]; */
struct ASTNode* parse_if_stmt(void)
{
    /* Obvious check */
    tok_assert_type(parser_get(), TOK_KEYWORD_IF);

    /* Retreive the line that statement was defined on */
    const LineNumber ln = parser_get_ln();

    /* Skip IF_KEYWORD token */
    parser_skip();

    /* Parse the condition */
    struct ASTNode* cond_node = __parse_cond();
    if (!cond_node) return NULL;

    /* Parse the body */
    struct ASTNode* then_node = __parse_body();
    if (!then_node) {
        /* Delete the condition node */
        ast_node_delete(cond_node);
        /* Fail */
        return NULL;
    }

    /* Create an cond/body statements vector */
    struct XVector* stmts = xvec_new(ast_node_free_f);

    /* Append the condition and the body */
    xvec_append(stmts, cond_node);
    xvec_append(stmts, then_node);

    /* Parse optinal 'elif' statements */
    parser_skip_newlines();
    if (!__parse_elif_pairs(stmts))
        goto out_free_stmts;

    /* Parse optional 'else' statement */
    struct ASTNode* else_node = NULL;
    /* Skip newlines */
    parser_skip_newlines();
    /* If we have an ELSE branch */
    if (parser_eq(TOK_KEYWORD_ELSE)) {
        /* Skip ELSE_KEYWORD token */
        parser_skip();

        /* Parse 'else' body */
        else_node = parse_stmt();
        if (!else_node) goto out_free_stmts;
    }

    /* Create a new IF AST node and return it */
    return mk_ast_node_if(ln, else_node, stmts);

out_free_stmts:
    /* Delete the allocated vector */
    xvec_delete(stmts);
    /* Fail */
    return NULL;
}
