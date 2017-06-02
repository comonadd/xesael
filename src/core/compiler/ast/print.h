/* File: print.h */
/* Creation Date: 2017-01-23 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* The AST/AST node print helpers */
/* NOTE: Those functions are only available when DEBUG == 1 */

#ifndef GSH_AST_PRINT_H
#define GSH_AST_PRINT_H

#if DEBUG
#include "core/compiler/ast/ast.h"
#include "core/compiler/ast/ast_node.h"
#include "lib/types.h"
NoRet ast_node_print(struct ASTNode const* node);
NoRet ast_nodes_print(struct XVector const* nodes);
NoRet ast_print(struct AST const* ast);
#endif /* DEBUG */

#endif /* GSH_AST_PRINT_H */
