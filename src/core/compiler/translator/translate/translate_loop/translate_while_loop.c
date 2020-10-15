/* File: translate_while_loop.c */
/* Creation Date: 2017-01-28 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* translate_while_node() function */

#include "core/compiler/translator/translate/translate_loop/translate_while_loop.h"

#include "core/compiler/ast/ast_node.h"
#include "core/compiler/translator/translate/translate.h"
#include "core/compiler/translator/translator.h"
#include "core/vm/instruction/instruction.h"
#include "lib/types.h"

bool translate_while_node(struct Translator* T, struct ASTNode* node)
{
  struct ASTNode* cond = node->data.while_.cond;
  struct ASTNode* body = node->data.while_.body;

  /* Retreive the start position of the loop */
  InstructionPosition start_pos = translator_get_ip(T);

  /* Compile condition node */
  if (!translate_node(T, cond)) return false;

  /* Reserve current instruction position */
  /* (for further replacement) */
  InstructionPosition enter_cond_check_instr_pos;
  enter_cond_check_instr_pos = translator_reserve_ip(T);

  /* Compile body node */
  if (!translate_node(T, body)) return false;

  /* Retreive the end position of the loop */
  InstructionPosition end_pos = translator_get_ip(T);

  /* Get the count of instructions in WHILE */
  /* loop body */
  InstructionPosition body_instr_count;
  body_instr_count = end_pos - start_pos;

  /* Emit a new JMPR instruction */
  Instruction goto_end_of_loop_instr = instr_jmpr_new(-body_instr_count);
  translator_append_instr(T, goto_end_of_loop_instr);

  /* Create a new JMPRCI instruction */
  Instruction enter_cond_check_instr =
    instr_jmprci_new(end_pos - enter_cond_check_instr_pos + 1, cond->rid);
  translator_insert_instr(
    T, enter_cond_check_instr_pos, enter_cond_check_instr);

  return true;
}
