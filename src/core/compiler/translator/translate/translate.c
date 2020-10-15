/* File: translate.c */
/* Creation Date: 2017-01-28 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* translate*() functions */

#include "core/compiler/translator/translate/translate.h"

#include "core/compiler/ast/ast.h"
#include "core/compiler/ast/ast_node.h"
#include "core/compiler/compiler.h"
#include "core/compiler/translator/translate/translate_assign.h"
#include "core/compiler/translator/translate/translate_binop.h"
#include "core/compiler/translator/translate/translate_call.h"
#include "core/compiler/translator/translate/translate_def/translate_const_def.h"
#include "core/compiler/translator/translate/translate_def/translate_enum_def.h"
#include "core/compiler/translator/translate/translate_def/translate_func_def.h"
#include "core/compiler/translator/translate/translate_def/translate_var_def.h"
#include "core/compiler/translator/translate/translate_if.h"
#include "core/compiler/translator/translate/translate_import.h"
#include "core/compiler/translator/translate/translate_lit.h"
#include "core/compiler/translator/translate/translate_loop/translate_for_loop.h"
#include "core/compiler/translator/translate/translate_loop/translate_generic_loop.h"
#include "core/compiler/translator/translate/translate_loop/translate_while_loop.h"
#include "core/compiler/translator/translate/translate_unop.h"
#include "core/compiler/translator/translate/translate_var.h"
#include "core/compiler/translator/translator.h"
#include "err.h"
#include "lib/types.h"

/********/
/* Main */
/********/

bool translate_node(struct Translator* T, struct ASTNode* node)
{
  /* If node is a literal */
  if (ast_node_is_lit(node)) return translate_lit_node(T, node);

  /* If node is a variable */
  if (node->type == AST_NODE_VAR) return translate_var_node(T, node);

  /* If node is a assignment */
  if (node->type == AST_NODE_ASSIGN) return translate_assign_node(T, node);

  /* If node is a binary operator */
  if (ast_node_is_binop(node)) return translate_binop_node(T, node);

  /* If node is a unary operator */
  if (ast_node_is_unop(node)) return translate_unop_node(T, node);

  switch (node->type) {
    case AST_NODE_BLOCK: return translate_nodes(T, node->data.block.stmts);
    case AST_NODE_IF: return translate_if_node(T, node);
    case AST_NODE_WHILE: return translate_while_node(T, node);
    case AST_NODE_IMPORT: return translate_import_node(T, node);
    case AST_NODE_VAR_DEF: return translate_var_def_node(T, node);
    case AST_NODE_CONST_DEF: return translate_const_def_node(T, node);
    case AST_NODE_FUNC_DEF: return translate_func_def_node(T, node);
    case AST_NODE_ENUM_DEF: return translate_enum_def_node(T, node);
    case AST_NODE_CALL: return translate_call_node(T, node);
    case AST_NODE_RETURN: goto out_ret_not_in_func;
    default:
      IMPOSSIBLE();
      /* Fail */
      return false;
  }

out_ret_not_in_func:
  compiler_err_set(node->ln,
                   "'return' statement used outside the function body");
  return false;
}

bool translate_nodes(struct Translator* T, struct XVector* nodes)
{
  /* If there is nothing to compile */
  if (!nodes) return NULL;

  /* For each node in the AST */
  for (uint64 i = 0; i < nodes->size; ++i) {
    /* Retreive the node */
    struct ASTNode* node = xvec_get(nodes, i);

    /* Compile the node */
    if (!translate_node(T, node)) return false;

    /* Free the registers */
    translator_free_regs(T);
  }

  /* Succeed */
  return true;
}

bool translate(struct ByteCode** output, struct AST* ast)
{
  /* If there is nothing to compile */
  if (ast->childs.size == 0) {
    /* Succeed */
    return NULL;
  }

  /* Initialize the translator */
  struct Translator T;
  translator_init(&T);

  /* Translate the given AST */
  struct XVector* nodes = &ast->childs;
  for (uint64 i = 0; i < nodes->size; ++i) {
    /* Translate the node */
    if (!translate_node(&T, xvec_get(nodes, i))) goto out;
    /* Free the registers */
    translator_free_regs(&T);
  }

  /* Append the EXIT instruction */
  translator_append_instr(&T, instr_exit_new());

  /* Convert the immediates into the vector */
  struct Object* const* immediates;
  immediates              = immediate_list_pack(&T.il);
  T.buf->immediates       = immediates;
  T.buf->immediates_count = T.il.size;

  /* Succeed */
  *output = T.buf;
  return true;

out:
  translator_deinit(&T);
  /* Fail */
  return false;
}
