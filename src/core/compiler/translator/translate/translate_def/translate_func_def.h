/* File: translate_func_def.h */
/* Creation Date: 2017-01-28 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* translate_func_def_node() function */

#ifndef TRANSLATE_FUNC_DEF_NODE_H
#define TRANSLATE_FUNC_DEF_NODE_H

#include "lib/types.h"

#include "core/compiler/ast/ast_node.h"

bool translate_func_def_node(struct ASTNode* node);

#endif /* TRANSLATE_FUNC_DEF_NODE_H */
