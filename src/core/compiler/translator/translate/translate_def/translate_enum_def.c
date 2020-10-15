/* File: translate_enum_def.c */
/* Creation date: 2017-02-04 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* translate_enum_def_node() function */

#include "core/compiler/translator/translate/translate_def/translate_enum_def.h"

#include "lib/macros.h"
#include "lib/types.h"

#include "core/compiler/ast/ast_node.h"

bool translate_enum_def_node(struct Translator* T, struct ASTNode* node)
{
  UNUSED(node);
  return false;
}
