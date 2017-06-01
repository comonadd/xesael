/* File: parse_atom.c */
/* Creation Date: 2017-01-30 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* parse_atom() function */

#include "core/compiler/parser/parse/parse_expr/parse_atom.h"

#include "core/object/bool/bool.h"
#include "core/compiler/token/token.h"
#include "core/compiler/ast/ast_node.h"
#include "core/compiler/compiler.h"
#include "core/compiler/parser/parser.h"
#include "core/compiler/parser/parse/parse_expr/parse_expr.h"

static struct ASTNode* parse_atom_call(
    struct Token const* tok)
{
    /* Save the line number that call was on */
    const LineNumber ln = parser_get_ln();

    /* Skip opening paren '(' */
    parser_skip();

    /* Retreive the arguments */
    struct XVector* args;
    args = xvec_new(ast_node_free_f);
    while (true) {
        /* Read the token */
        struct Token const* arg_tok = parser_get();
        switch (arg_tok->type) {
            case TOK_SEP_COMMA:
                parser_skip();
                break;
            case TOK_SEP_RPAREN:
                parser_skip();
                goto done;
            case TOK_INVALID: goto out_unclosed_paren;
            default: {
                struct ASTNode* arg = parse_expr();
                if (!arg) goto out;
                xvec_append(args, arg);
            }
        }
    }

done:
    /* Succeed */
    return mk_ast_node_call(ln, tok->val, args);

out_unclosed_paren:
    compiler_err_set(ln, "unclosed parenthesis");
    goto out;

out:
    xvec_delete(args);
    /* Fail */
    return NULL;
}

struct ASTNode* parse_atom(void)
{
    /* If we have expression like "2 54" */
    if (tok_is_id(parser_lookback())) {
        /* Set appropriate error message */
        parser_err_set_invalid_syntax();
        /* Fail */
        return NULL;
    }

    /* Retreive the current token */
    struct Token const* tok = parser_read();

    /* The result of computation */
    struct ASTNode* res;

    /* If token is a prefix unary operator */
    if (tok_is_prefix_unop(tok)) {
        /* Save the line number that expression was defined on */
        const LineNumber ln = parser_get_ln();

        /* Parse the operand */
        struct ASTNode* op = parse_atom();
        if (!op) return NULL;

        /* Retreive the AST node type */
        enum ASTNodeType unop_type;
        unop_type = tt_unop2astt(tok->type);

        /* Make a new unary operator node */
        return mk_ast_node_unop(ln, unop_type, op);
    }

    switch (tok->type) {
        case TOK_INUMLIT:
            res = mk_ast_node_lit(
                AST_NODE_INUMLIT,
                parser_get_ln(),
                tok->val);
            break;

        case TOK_DNUMLIT:
            res = mk_ast_node_lit(
                AST_NODE_DNUMLIT,
                parser_get_ln(),
                tok->val);
            break;

        case TOK_STRLIT:
            res = mk_ast_node_lit(
                AST_NODE_STRLIT,
                parser_get_ln(),
                tok->val);
            break;

        case TOK_KEYWORD_TRUE:
            res = mk_ast_node_lit(
                AST_NODE_BOOLLIT,
                parser_get_ln(),
                CAST(&OBJ_TRUE, struct Object*));
            break;

        case TOK_KEYWORD_FALSE:
            res = mk_ast_node_lit(
                AST_NODE_BOOLLIT,
                parser_get_ln(),
                CAST(&OBJ_FALSE, struct Object*));
            break;

        case TOK_ID: {
            /* If token that goes after the */
            /* ID token is a opening '(' paren */
            if (parser_eq(TOK_SEP_LPAREN)) {
                res = parse_atom_call(tok);
                break;
            }

            res = mk_ast_node_var(
                parser_get_ln(),
                tok->val);
            break;
        }

        case TOK_SEP_LPAREN: {
            /* Parse the expression in the parentheses */
            res = parse_expr();

            /* If there is no right parenthesis */
            if (!parser_eat(TOK_SEP_RPAREN)) {
                ast_node_delete(res);
                /* Fail */
                return NULL;
            }
            break;
        }

        default: {
            compiler_err_setf(
                parser_get_ln(),
                "expression expected, got \"%s\"",
                tok2str(tok));
            return NULL;
        }
    }

    tok = parser_get();
    if (tok_is_postfix_unop(tok)) {
        /* Skip it */
        parser_skip();

        /* Make a new unary operator node */
        res = mk_ast_node_unop(
            parser_get_ln(),
            tt_unop2astt(tok->type),
            res);
    }

    /* Succeed */
    return res;
}
