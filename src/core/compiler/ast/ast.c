/* File: ast.c */
/* Creation date: 15.12.2016 */
/* Author: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* AST implementation */

#include "core/compiler/ast/ast.h"

#include "lib/types.h"
#include "lib/macros.h"
#include "lib/xmalloc.h"
#include "lib/xvec.h"
#include "err.h"
#include "core/object/object.h"

/***********/
/* Methods */
/***********/

NoRet ast_init(struct AST* ast)
{
    /* Obvious check */
    DASSERT(ast, "");
    xvec_init(&ast->childs, ast_node_free_f);
}

NoRet ast_deinit(struct AST* ast)
{
    /* Obvious check */
    DASSERT(ast, "");
    while (ast->childs.size != 0) {
        struct ASTNode* node;
        node = xvec_get(&ast->childs, ast->childs.size - 1);
        ast_node_delete(node);
        --ast->childs.size;
    }
    xvec_deinit(&ast->childs);
}

struct AST* ast_new(void)
{
    /* Allocate a new AST */
    struct AST* ast = xmalloc(sizeof(*ast));
    if (!ast) {
        err_set_not_enough_memory();
        /* Fail */
        return NULL;
    }

    /* Initialize the newly allocated AST */
    ast_init(ast);
    return ast;
}

NoRet ast_delete(struct AST* ast)
{
    DASSERT(ast, "");
    if (!ast) return;
    ast_deinit(ast);
    xfree(ast);
}

NoRet ast_append(
    struct AST* ast,
    struct ASTNode* node)
{
    DASSERT(ast, "");
    DASSERT(node, "");
    xvec_append(&ast->childs, node);
}
