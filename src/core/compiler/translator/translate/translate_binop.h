/* File: translate_binop.h */
/* Creation Date: 2017-01-28 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* translate_binop_node() function */

#ifndef TRANSLATE_BINOP_H
#define TRANSLATE_BINOP_H

#include "lib/types.h"
#include "core/compiler/ast/ast_node.h"

bool translate_binop_node(struct ASTNode* node);

#endif /* TRANSLATE_BINOP_H */
