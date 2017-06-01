/* File: parse_stmt_block.c */
/* Creation Date: 2017-01-30 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* parse_block_stmt() function */

#include "core/compiler/parser/parse/parse_stmt/parse_block_stmt.h"

#include "lib/types.h"

#include "core/compiler/compiler.h"
#include "core/compiler/parser/parser.h"
#include "core/compiler/parser/parse/parse_stmt/parse_stmt.h"

/* stmt_block = LBRACE {stmt} RBRACE; */
struct ASTNode* parse_block_stmt(void)
{
    /* Obvious check */
    tok_assert_type(parser_get(), TOK_SEP_LBRACE);

    /* Save the line that block was defined on */
    const LineNumber ln = parser_get_ln();

    /* Skip the opening brace '{' token */
    parser_skip();

    /* Check that we have an empty block */
    {
        /* Skip all the newlines */
        parser_skip_newlines();

        /* If we actually have an empty block */
        if (parser_eq(TOK_SEP_RBRACE)) {
            compiler_err_set(ln, "empty block");
            return NULL;
        }
    }

    /* Allocate and initialize a new vector */
    /* that will contain the body of the block */
    struct XVector* nodes = xvec_new(ast_node_free_f);

    /* While current token is not the closing brace '}' token */
    struct ASTNode* node;
    while (parser_neq(TOK_SEP_RBRACE)) {
        /* Skip all the newlines */
        parser_skip_newlines();

        switch (parser_get()->type) {
            case TOK_SEP_RBRACE: goto done;
            case TOK_INVALID: goto out_unclosed_block_free_nodes;
            default: PASS();
        }

        /* Parse a statement */
        node = parse_stmt();
        if (!node) goto out_free_nodes;

        /* Append a new statement to the body */
        xvec_append(nodes, node);
    }

done:
    /* Skip the closing brace '}' token */
    parser_skip();

    /* Create and return a new BLOCK node */
    return mk_ast_node_block(ln, nodes);

out_unclosed_block_free_nodes:
    compiler_err_set(ln, "unclosed block statement");
    goto out_free_nodes;

out_free_nodes:
    xvec_delete(nodes);
    return NULL;
}
