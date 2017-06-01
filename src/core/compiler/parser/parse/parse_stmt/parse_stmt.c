/* File: parse_stmt.c */
/* Creation Date: 2017-01-30 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* parse_stmt() function */

#include "core/compiler/parser/parse/parse_stmt/parse_stmt.h"

#include "err.h"

#include "core/compiler/token/token.h"
#include "core/compiler/ast/ast_node.h"
#include "core/compiler/parser/parser.h"
#include "core/compiler/parser/parse/parse_stmt/parse_block_stmt.h"
#include "core/compiler/parser/parse/parse_stmt/parse_def_stmt/parse_def_stmt.h"
#include "core/compiler/parser/parse/parse_stmt/parse_flow_stmt/parse_flow_stmt.h"
#include "core/compiler/parser/parse/parse_stmt/parse_loop_stmt/parse_loop_stmt.h"
#include "core/compiler/parser/parse/parse_stmt/parse_import_stmt.h"
#include "core/compiler/parser/parse/parse_expr/parse_expr.h"

/* stmt = STMT_BLOCK   */
/*      | def_stmt     */
/*      | flow_stmt    */
/*      | loop_stmt    */
/*      | import_stmt  */
/*      | expr;        */
struct ASTNode* parse_stmt(void)
{
    /* Retreive the current token */
    struct Token const* tok = parser_get();

    /* If we reached the end of the stream */
    if (tok_is_invalid(tok)) {
        /* Done */
        err_reset();
        return NULL;
    }

    /* If token is a statement separator */
    if (tok_is_stmt_sep(tok)) {
        switch (tok->type) {
            case TOK_SEP_NEWLINE:
                /* Increment current line number */
                parser_incr_ln();
            default: PASS();
        }

        /* Skip the separator */
        parser_skip();

        /* Parse next statement */
        return parse_stmt();
    }

    if (tok->type == TOK_SEP_LBRACE) {
        /* Parse block statement */
        return parse_block_stmt();
    } else if (tok_can_start_def_stmt(tok)) {
        return parse_def_stmt();
    } else if (tok_can_start_flow_stmt(tok)) {
        return parse_flow_stmt();
    } else if (tok_can_start_loop_stmt(tok)) {
        return parse_loop_stmt();
    }

    switch (tok->type) {
        case TOK_KEYWORD_IMPORT: return parse_import_stmt();
        default: return parse_expr();
    }
}
