/* File: translate_enum_def.h */
/* Creation date: 2017-02-04 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* translate_enum_def_node() function */

#ifndef TRANSLATE_ENUM_DEF_H
#define TRANSLATE_ENUM_DEF_H

#include "lib/types.h"

#include "core/compiler/ast/ast_node.h"

bool translate_enum_def_node(struct ASTNode* node);

#endif /* TRANSLATE_ENUM_DEF_H */
