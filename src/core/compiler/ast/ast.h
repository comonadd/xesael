/* File: ast.h */
/* Creation date: 15.12.2016 */
/* Author: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* AST implementation */

#ifndef GSH_AST_H
#define GSH_AST_H

#include "lib/types.h"
#include "lib/xvec.h"
#include "core/compiler/ast/ast_node.h"

struct AST {
    struct XVector childs;
};

/***********/
/* Methods */
/***********/

NoRet ast_init(struct AST* ast);

NoRet ast_deinit(struct AST* ast);

struct AST* ast_new(void);

NoRet ast_delete(struct AST* ast);

NoRet ast_append(
    struct AST* ast,
    struct ASTNode* node);

struct ASTNode* ast_get_child(
    struct AST* ast,
    const uint64 i);

#endif /* GSH_AST_H */
