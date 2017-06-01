/* File: parse.c */
/* Creation date: 2017-02-04 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* parse() function */

#include "core/compiler/parser/parse/parse.h"

#include "lib/xvec.h"

#include "core/compiler/token/token.h"
#include "core/compiler/ast/ast.h"
#include "core/compiler/parser/parser.h"
#include "core/compiler/parser/parse/parse_stmt/parse_stmt.h"

/**********************/
/* Private prototypes */
/**********************/

static struct AST* __parse(void);

/*********************/
/* Private functions */
/*********************/

static struct AST* __parse(void)
{
    /* While current token in the stream */
    /* is not the last token in the stream */
    struct ASTNode* node;
    while (parser_neq(TOK_INVALID)) {
        /* Parse the statement */
        node = parse_stmt();

        /* If result is NULL */
        if (!node) {
            /* If error is not set */
            if (err_is_ok()) {
                /* We are done */
                goto done;
            }

            /* We got an error */
            goto out;
        }

        /* Append resulting node to AST */
        ast_append(parser.ast, node);
    }

done:
    /* Succeed */
    return parser.ast;

out:
    /* Detete the allocated AST */
    ast_delete(parser.ast);
    /* Fail */
    return NULL;
}

/********/
/* Main */
/********/

struct AST* parse(struct XVector const* toks)
{
    /* Initialize the parser */
    parser_init(toks);
    /* Actually parse */
    return __parse();
}
