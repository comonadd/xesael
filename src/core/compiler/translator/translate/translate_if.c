/* File: translate_if.c */
/* Creation Date: 2017-01-28 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */

#include "core/compiler/translator/translate/translate_if.h"

#include "core/compiler/ast/ast_node.h"
#include "core/compiler/translator/translate/translate.h"
#include "core/compiler/translator/translator.h"
#include "core/vm/instruction/instruction.h"
#include "lib/types.h"

bool translate_if_node(struct Translator* T, struct ASTNode* node)
{
  /* This is the array that we will put */
  /* positions that we want to replace later */
  /* with IP - pos_to_replace[i] */
  static const uint8 POS_TO_REPLACE_ALLOC_SIZE = 64;
  uint64 pos_to_replace_capacity               = POS_TO_REPLACE_ALLOC_SIZE;
  uint64 pos_to_replace_count                  = 0;
  InstructionPosition* pos_to_replace;
  pos_to_replace = xmalloc(pos_to_replace_capacity * sizeof(*pos_to_replace));

  /* Go through the vector of IF node statements */
  /* Stepping by 2 and grabbing new condition and body nodes */
  {
    struct ASTNode* cond;
    struct ASTNode* body;
    uint64 i = 0;
    while (i < node->data.if_.stmts->size) {
      /* Get the condition node and compile it */
      cond = xvec_get(node->data.if_.stmts, i);
      if (!translate_node(T, cond)) goto out;

      /* Reserve one instruction for further filling */
      /* (and save its position) */
      InstructionPosition enter_cond_check_instr_pos = translator_reserve_ip(T);

      /* Get the body node and compile it */
      body = xvec_get(node->data.if_.stmts, i + 1);
      if (!translate_node(T, body)) goto out;

      /* Reserve one instruction for further filling */
      /* (and save its position) */
      InstructionPosition body_last_ip = translator_reserve_ip(T);

      /* If we not allocated memory in the array for saved positions */
      if (pos_to_replace_count >= pos_to_replace_capacity) {
        /* Allocate more */
        pos_to_replace_capacity += POS_TO_REPLACE_ALLOC_SIZE;
        pos_to_replace = xrealloc(
          pos_to_replace, pos_to_replace_capacity * sizeof(*pos_to_replace));
      }

      /* Save last instruction position in the body */
      /* (for further filling) */
      pos_to_replace[pos_to_replace_count++] = body_last_ip;

      /* Retreive the end position of the body */
      InstructionPosition end_pos = translator_get_ip(T);

      /* Create new JMPRCI instruction */
      Instruction instr =
        instr_jmprci_new(end_pos - enter_cond_check_instr_pos, cond->rid);

      /* Insert it to the saved position */
      translator_insert_instr(T, enter_cond_check_instr_pos, instr);

      /* Go to the next cond/body pair in the vector */
      i += 2;
    }
  }

  InstructionPosition end_pos = translator_get_ip(T);

  for (uint64 i = 0; i < pos_to_replace_count; ++i) {
    Instruction instr;
    instr = instr_jmpr_new(end_pos - pos_to_replace[i]);
    translator_insert_instr(T, pos_to_replace[i], instr);
  }

  xfree(pos_to_replace);
  /* Succeed */
  return true;

out:
  xfree(pos_to_replace);
  /* Fail */
  return false;
}
