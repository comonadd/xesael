/* File: translate_import.h */
/* Creation Date: 2017-01-28 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* translate_import_node() function */

#ifndef TRANSLATE_IMPORT_H
#define TRANSLATE_IMPORT_H

#include "core/compiler/translator/translator.h"
#include "lib/types.h"

bool translate_import_node(struct Translator* T, struct ASTNode* node);

#endif /* TRANSLATE_IMPORT_H */
