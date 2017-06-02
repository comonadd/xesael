/* File: translate_call.c */
/* Creation Date: 2017-01-28 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* translate_call_node() function */

#include "core/compiler/translator/translate/translate_call.h"

#include "lib/types.h"

#include "core/compiler/ast/ast_node.h"
#include "core/compiler/translator/translate/translate.h"
#include "core/compiler/translator/translator.h"
#include "core/vm/instruction/instruction.h"

bool translate_call_node(struct ASTNode* node)
{
  /* Obvious check */
  DASSERT(node->type == AST_NODE_CALL, "");

  /* Retreive the first argument register ID */
  const RegisterID args_start_rid = translator_get_curr_rid();

  /* Create and insert a function immediate name */
  struct Immediate* func_immediate;
  func_immediate =
    translator_immediate_insert(node->data.call.name->data.str.val, func_obj);

  /* For each argument in the arguments vector */
  for (uint64 i = 0; i < node->data.call.args->size; ++i) {
    /* Save the current register free register ID */
    RegisterID saved_rid = translator_get_curr_rid();

    /* Compile argument */
    struct ASTNode* arg_node = xvec_get(node->data.call.args, i);
    if (!translate_node(arg_node)) return false;

    /* Retreive the register id of the argument */
    /* RegisterID arg_rid = arg_node->rid; */
    /* Set the compiler's current free register ID to the saved_rid + 1 */
    translator_set_curr_rid(saved_rid + 1);
  }

  /* Create a new CALL instruction */
  Instruction instr = instr_call_new(
    func_immediate->id, args_start_rid, node->data.call.args->size);

  /* Append a newly created CALL instruction */
  translator_append_instr(instr);

  /* Succeed */
  return true;
}
