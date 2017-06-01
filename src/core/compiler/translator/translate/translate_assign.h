/* File: translate_assign.h */
/* Creation date: 2017-02-06 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* translate_assign_node() function */

#ifndef TRANSLATE_ASSIGN_H
#define TRANSLATE_ASSIGN_H

#include "lib/types.h"

#include "core/compiler/translator/translator.h"
#include "core/compiler/ast/ast_node.h"

bool translate_assign_node(
    struct Translator* T,
    struct ASTNode* node);

#endif /* TRANSLATE_ASSIGN_H */
