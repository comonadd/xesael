/* File: translate_unop.c */
/* Creation Date: 2017-01-28 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* translate_unop_node() function */

#include "translate_unop.h"

#include "core/compiler/ast/ast_node.h"
#include "core/compiler/translator/translate/translate.h"
#include "core/compiler/translator/translator.h"
#include "lib/types.h"

bool translate_unop_node(struct ASTNode* node)
{
  /* Retreive the register ID in which result will be stored */
  node->rid = translator_get_free_reg();

  /* Retreive and compile the operand */
  struct ASTNode* op = node->data.unop.op;
  if (!translate_node(op)) return false;

  /* Whichever operator we have, create a new instruction */
  enum InstructionOpCode code;
  code = unop_instr_code_from_ast_node(node);

  /* Emit a new instruction */
  const Instruction instr = instr_unop_new(code, op->rid, node->rid);
  translator_append_instr(instr);

  /* Succeed */
  return true;
}
