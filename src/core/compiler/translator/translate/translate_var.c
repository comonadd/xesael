/* File: translate_var.c */
/* Creation Date: 2017-01-28 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* translate_var_node() function */

#include "core/compiler/translator/translate/translate_var.h"

#include "err.h"

#include "lib/types.h"

#include "core/compiler/ast/ast_node.h"
#include "core/compiler/compiler.h"
#include "core/compiler/translator/translator.h"

bool translate_var_node(struct Translator* T, struct ASTNode* node)
{
  /* Assign the register ID of a node */
  node->rid = translator_get_free_reg(T);

  /* Retreive the name of a name */
  char* name = node->data.var.name->data.str.val;

  /* Search for the name symbol */
  struct Symbol* sym = translator_st_lookup(T, name);

  /* If we failed to found such a symbol */
  if (!sym || (sym->type != SYM_VAR)) {
    /* Set the error message */
    compiler_err_setf(
      node->ln, "variable with name \"%s\" is not defined", name);
    /* Fail */
    return false;
  }

  /* Create VARLOAD instruction */
  Instruction instr;
  instr = instr_varload_new(sym->data.var_name_immediate_id, node->rid);

  /* Append it */
  translator_append_instr(T, instr);

  /* Succeed */
  return true;
}
