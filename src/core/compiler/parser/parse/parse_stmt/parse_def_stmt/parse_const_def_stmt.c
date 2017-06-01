/* File: parse_const_def_stmt.c */
/* Creation Date: 2017-01-30 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* parse_const_def_stmt() function */

#include "core/compiler/parser/parse/parse_stmt/parse_def_stmt/parse_const_def_stmt.h"

#include "core/compiler/ast/ast_node.h"
#include "core/compiler/parser/parser.h"
#include "core/compiler/parser/parse/parse_stmt/parse_def_stmt/parse_initializator.h"

/* constdef_stmt = CONST_KEYWORD identifier EQUALS initializator; */
/* TODO: Implement this */
struct ASTNode* parse_const_def_stmt(void)
{
    /* Obvious check */
    tok_assert_type(parser_get(), TOK_KEYWORD_CONST);

    /* Save the line number that constant was defined on */
    const LineNumber ln = parser_get_ln();

    /* Skip the CONST_KEYWORD token */
    parser_skip();

    /* Retreive the name of the constant */
    struct Token const* name_tok = parser_eat(TOK_ID);
    if (!name_tok) return NULL;
    struct Object* name = name_tok->val;

    /* Eat the EQUALS token */
    if (!parser_eat(TOK_ASSIGN)) return NULL;

    /* Parse the initializator */
    struct ASTNode* initializator = parse_initializator();
    if (!initializator) return NULL;

    /* Create a new CONST_DEF AST node */
    return mk_ast_node_const_def(ln, name, initializator);
}
