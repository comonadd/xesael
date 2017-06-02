/* File: translate_call.h */
/* Creation Date: 2017-01-28 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* translate_call_node() function */

#ifndef TRANSLATE_CALL_H
#define TRANSLATE_CALL_H

#include "core/compiler/ast/ast_node.h"
#include "lib/types.h"

bool translate_call_node(struct ASTNode* node);

#endif /* TRANSLATE_CALL_H */
