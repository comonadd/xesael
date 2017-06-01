/* File: translate_while_loop.h */
/* Creation Date: 2017-01-28 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* translate_while_node() function */

#ifndef TRANSLATE_WHILE_LOOP_H
#define TRANSLATE_WHILE_LOOP_H

#include "lib/types.h"

#include "core/compiler/ast/ast_node.h"

bool translate_while_node(struct ASTNode* node);

#endif /* TRANSLATE_WHILE_LOOP_H */
