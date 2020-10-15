/* File: translate_const_def.h */
/* Creation Date: 2017-01-30 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* translate_const_def_node() function */

#ifndef TRANSLATE_CONST_DEF_H
#define TRANSLATE_CONST_DEF_H

#include "core/compiler/ast/ast_node.h"
#include "core/compiler/translator/translator.h"

bool translate_const_def_node(struct Translator* T, struct ASTNode* node);

#endif /* TRANSLATE_CONST_DEF_H */
