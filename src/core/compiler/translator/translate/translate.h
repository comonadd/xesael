/* File: translate.h */
/* Creation Date: 2017-01-28 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* translate_node(), translate_nodes() and translate_ast() functions */

#ifndef TRANSLATE_H
#define TRANSLATE_H

#include "lib/types.h"

#include "core/compiler/ast/ast.h"
#include "core/compiler/ast/ast_node.h"
#include "core/compiler/translator/translator.h"

bool translate_node(struct Translator* T, struct ASTNode* node);
struct ByteCode* translate_nodes(struct XVector* nodes);
struct ByteCode* translate(struct AST* ast);

#endif /* TRANSLATE_H */
