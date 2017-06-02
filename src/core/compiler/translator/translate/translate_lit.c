/* File: translate_lit.c */
/* Creation Date: 2017-01-28 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* The translate_lit_node() function */

#include "core/compiler/translator/translate/translate_lit.h"

#include "core/compiler/ast/ast_node.h"
#include "core/compiler/translator/translator.h"
#include "lib/types.h"

bool translate_lit_node(struct ASTNode* node)
{
  /* Obvious check */
  DASSERT(ast_node_is_lit(node), "");

  /* Assign the register ID of a node */
  node->rid = translator_get_free_reg();

  /* Retreive the node value */
  struct Object* node_val = node->data.lit.val;

  /* Retreive the object that new immediate will contain */
  struct Object* immediate_obj = ast_node_lit_get_obj(node);

  /* Insert a immediate in the compiler's constants list */
  struct Immediate* immediate;
  immediate =
    translator_immediate_insert(node_val->data.str.val, immediate_obj);

  /* Create and append a new LOAD instruction */
  Instruction instr = instr_load_new(immediate->id, node->rid);
  translator_append_instr(instr);

  /* Succeed */
  return true;
}
