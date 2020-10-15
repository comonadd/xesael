/* File: translate_func_def.c */
/* Creation Date: 2017-01-28 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* translate_func_def_node() function */

#include "core/compiler/translator/translate/translate_def/translate_func_def.h"

#include "core/compiler/ast/ast_node.h"
#include "core/compiler/compiler.h"
#include "core/compiler/sym/sym.h"
#include "core/compiler/translator/translate/translate.h"
#include "core/compiler/translator/translator.h"
#include "core/object/func/func.h"
#include "err.h"
#include "lib/types.h"

static bool fill_func_obj(struct Translator* T,
                          struct XVector* nodes,
                          struct Object* obj);

static bool fill_func_obj(struct Translator* T,
                          struct XVector* nodes,
                          struct Object* obj)
{
  /* For each node in the nodes vector */
  for (uint64 i = 0; i < nodes->size; ++i) {
    /* Retreive the node */
    struct ASTNode* body_node = xvec_get(nodes, i);
    switch (body_node->type) {
      case AST_NODE_RETURN:
        /* Compile the return expression */
        /* Append instuction "RETURN" */
        break;
      default:
        if (!translate_node(T, body_node)) return false;
    }
  }
  /* Succeed */
  return true;
}

bool translate_func_def_node(struct Translator* T, struct ASTNode* node)
{
  /* Get the name of a function */
  char* name;
  name = ast_node_func_def_name(node)->data.str.val;

  /* Create a new function object */
  struct Object* obj = obj_func_new();

  /* Insert a new function immediate */
  ImmediateID func_immediate_id;
  func_immediate_id = translator_immediate_insert(T, name, obj)->id;

  /* Create a new function symbol */
  struct Symbol* sym;
  sym = sym_func_new(name, func_immediate_id);

  /* Try to insert it to the symbol table */
  if (!translator_st_insert(T, sym)) {
    /* If such a name is already defined */
    /* Set the appropriate error message */
    compiler_err_setf(node->ln, "name \"%s\" is already taken", name);
    /* Fail */
    return false;
  }

  /* Allocate a new bytecode array for the function body */
  struct ByteCode* bc = bytecode_new();

  struct ASTNode* body  = node->data.func_def.body;
  struct XVector* nodes = body->data.block.stmts;
  fill_func_obj(T, nodes, obj);

  /* Succeed */
  return true;
}
