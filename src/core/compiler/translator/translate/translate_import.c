/* File: translate_import.c */
/* Creation Date: 2017-01-28 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* translate_import_node() function */

#include "core/compiler/translator/translate/translate_import.h"

#include "lib/types.h"

#include "core/compiler/translator/translator.h"

bool translate_import_node(struct ASTNode* node)
{
  /* Retreive the name to import */
  struct Object* name_obj = node->data.import.val;
  char* name              = name_obj->data.str.val;

  /* Append a import name immediate */
  ImmediateID name_immediate_id;
  name_immediate_id = translator_immediate_insert(name, name_obj)->id;

  /* Emit the new IMPORT instruction */
  const Instruction instr = instr_import_new(name_immediate_id);
  translator_append_instr(instr);

  /* Succeed */
  return true;
}
