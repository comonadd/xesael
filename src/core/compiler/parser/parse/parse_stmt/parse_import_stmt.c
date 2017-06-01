/* File: parse_import_stmt.c */
/* Creation Date: 2017-01-30 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* parse_import_stmt() function */

#include "core/compiler/parser/parse/parse_stmt/parse_import_stmt.h"

#include "lib/types.h"

#include "core/compiler/token/token.h"
#include "core/compiler/ast/ast_node.h"
#include "core/compiler/parser/parser.h"

/* import_stmt = IMPORT_KEYWORD identifier; */
struct ASTNode* parse_import_stmt(void)
{
    /* Obvious check */
    tok_assert_type(parser_get(), TOK_KEYWORD_IMPORT);

    /* Save the current line number */
    const LineNumber ln = parser_get_ln();

    /* Skip IMPORT_KEYWORD token */
    parser_skip();

    /* Get the token that represents a name to import */
    struct Token const* import_name_tok;
    import_name_tok = parser_eat(TOK_ID);
    if (!import_name_tok) return NULL;
    if (!parser_eat_sep_or_end()) return NULL;

    /* Get the object that contains a name to import */
    struct Object* import_name;
    import_name = import_name_tok->val;

    /* Create and return a new IMPORT AST node */
    return mk_ast_node_import(ln, import_name);
}
