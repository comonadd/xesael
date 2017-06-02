/* File: translate_var_def.c */
/* Creation Date: 2017-01-28 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* translate_var_def_node() function */

#include "translate_var_def.h"

#include "err.h"

#include "lib/types.h"

#include "core/compiler/ast/ast_node.h"
#include "core/compiler/compiler.h"
#include "core/compiler/translator/translate/translate.h"
#include "core/compiler/translator/translator.h"

bool translate_var_def_node(struct ASTNode* node)
{
  /* Retreive and compile the initializator node */
  struct ASTNode* initializator;
  initializator = node->data.var_def.initializator;
  if (!translate_node(initializator)) return false;

  /* Retreive the variable name */
  struct Object* name_obj = node->data.var_def.name;
  char* name              = name_obj->data.str.val;

  /* Create a new immediate that will store the variable name */
  struct Immediate* name_immediate;
  name_immediate = translator_immediate_insert(name, name_obj);

  /* Create a new variable symbol and try to insert it */
  /* to the symbol table */
  struct Symbol* sym = sym_var_new(name, name_immediate->id);
  translator_st_insert(sym);

  /* If failed */
  if (!sym) {
    /* Set the appropriate error message */
    compiler_err_setf(
      node->ln, "variable with name \"%s\" is already defined", name);
    /* Fail */
    return false;
  }

  /* Emit a new VARSET instruction */
  Instruction instr = instr_varset_new(name_immediate->id, initializator->rid);
  translator_append_instr(instr);

  /* Succeed */
  return true;
}
