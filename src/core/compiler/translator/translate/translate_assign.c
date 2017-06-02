/* File: translate_assign.c */
/* Creation date: 2017-02-06 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */

#include "core/compiler/translator/translate/translate_assign.h"

#include "lib/types.h"

#include "core/compiler/ast/ast_node.h"
#include "core/compiler/compiler.h"
#include "core/compiler/translator/translator.h"

bool translate_assign_node(struct Translator* T, struct ASTNode* node)
{
  /* Retreive the register for storing the result of assignment */
  node->rid = translator_get_free_reg(T);

  /* Retreive the left operand of the assignment */
  struct ASTNode* lop = node->data.binop.lop;

  /* If we got something else than the variable */
  /* on the left-hand side */
  if (lop->type != AST_NODE_VAR) {
    /* Set the appropriate error message */
    compiler_err_set(node->ln,
                     "assignment operator requires lvalue as a left operand");
    /* Fail */
    return false;
  }

  /* Retreive the name of the variable */
  struct Object* name_obj = lop->data.var.name;
  char* name              = name_obj->data.str.val;

  /* Retreive the variable symbol */
  struct Symbol* sym = translator_st_lookup(T, name);

  /* If no such found */
  if (!sym) {
    /* Set the error message */
    compiler_err_setf(
      node->ln, "variable with name \"%s\" is not defined", name);
    /* Fail */
    return false;
  }

  if (sym->type == SYM_CONST) {
    /* Set the error message */
    compiler_err_setf(
      node->ln, "tried to change the value of the constant \"%s\"", name);
    /* Fail */
    return false;
  }

  /* Retreive the right operand of the assignment */
  struct ASTNode* rop = node->data.binop.rop;
  if (!translate_node(T, rop)) return false;

  /* Emit the VARSET instruction */
  Instruction instr =
    instr_varset_new(sym->data.var_name_immediate_id, rop->rid);
  translator_append_instr(instr);

  /* Succeed */
  return true;
}
