/* File: translate_for_loop.h */
/* Creation date: 2017-02-04 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* translate_for_loop_node() function */

#ifndef TRANSLATE_FOR_LOOP_H
#define TRANSLATE_FOR_LOOP_H

#include "lib/types.h"

#include "core/compiler/ast/ast_node.h"

bool translate_for_loop_node(struct ASTNode* node);

#endif /* TRANSLATE_FOR_LOOP_H */
