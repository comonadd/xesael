/* File: print.c */
/* Creation Date: 2017-01-23 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* The AST/AST node print helpers */
/* NOTE: Those functions are only available when DEBUG == 1 */

#include "core/compiler/ast/print.h"

#include <stdio.h>

#include "core/compiler/ast/ast.h"
#include "core/compiler/ast/ast_node.h"
#include "core/object/bool/bool.h"
#include "core/object/object.h"
#include "core/object/object.h"
#include "lib/macros.h"
#include "lib/types.h"
#include "lib/xmalloc.h"

#if DEBUG

/**********************/
/* Private prototypes */
/**********************/

static NoRet ast_nodes_print_(struct XVector const* nodes,
                              const uint64 indentation_level);

static NoRet ast_node_print_(struct ASTNode const* node,
                             const uint16 indentation_level);

NoRet ast_print_(struct AST const* ast, const uint64 indentation_level);

/*************/
/* Constants */
/*************/

static const uint8 SPACES_PER_LEVEL = 4;

static char* ast_nodet_lit_2str(const enum ASTNodeType type)
{
  switch (type) {
    case AST_NODE_INUMLIT: return "AST_NODE_INUMLIT";
    case AST_NODE_DNUMLIT: return "AST_NODE_DNUMLIT";
    case AST_NODE_BOOLLIT: return "AST_NODE_BOOLLIT";
    case AST_NODE_STRLIT: return "AST_NODE_STRLIT";
    default: IMPOSSIBLE(); return NULL;
  }
}

static char* ast_nodet_unop_2str(const enum ASTNodeType type)
{
  switch (type) {
    case AST_NODE_NEG: return "AST_NODE_NEG";
    case AST_NODE_LNOT: return "AST_NODE_LNOT";
    case AST_NODE_BNOT: return "AST_NODE_BNOT";
    case AST_NODE_LEN: return "AST_NODE_LEN";
    case AST_NODE_INCR: return "AST_NODE_INCR";
    case AST_NODE_DECR: return "AST_NODE_DECR";
    default: IMPOSSIBLE(); return NULL;
  }
}

static char* ast_nodet_binop_2str(const enum ASTNodeType type)
{
  switch (type) {
    case AST_NODE_ADD: return "AST_NODE_ADD";
    case AST_NODE_SUB: return "AST_NODE_SUB";
    case AST_NODE_MUL: return "AST_NODE_MUL";
    case AST_NODE_DIV: return "AST_NODE_DIV";
    case AST_NODE_MOD: return "AST_NODE_MOD";
    case AST_NODE_POW: return "AST_NODE_POW";
    case AST_NODE_EQ: return "AST_NODE_EQ";
    case AST_NODE_NEQ: return "AST_NODE_NEQ";
    case AST_NODE_LT: return "AST_NODE_LT";
    case AST_NODE_GT: return "AST_NODE_GT";
    case AST_NODE_LTE: return "AST_NODE_LTE";
    case AST_NODE_GTE: return "AST_NODE_GTE";
    case AST_NODE_LAND: return "AST_NODE_LAND";
    case AST_NODE_LOR: return "AST_NODE_LOR";
    case AST_NODE_BAND: return "AST_NODE_BAND";
    case AST_NODE_BOR: return "AST_NODE_BOR";
    case AST_NODE_BXOR: return "AST_NODE_BXOR";
    case AST_NODE_LS: return "AST_NODE_LS";
    case AST_NODE_RS: return "AST_NODE_RS";
    case AST_NODE_ASSIGN: return "AST_NODE_ASSIGN";
    default: IMPOSSIBLE(); return NULL;
  }
}

static char* ast_nodet2str(const enum ASTNodeType type)
{
  if (ast_nodet_is_lit(type))
    return ast_nodet_lit_2str(type);
  else if (ast_nodet_is_unop(type))
    return ast_nodet_unop_2str(type);
  else if (ast_nodet_is_binop(type))
    return ast_nodet_binop_2str(type);
  else {
    switch (type) {
      case AST_NODE_VAR: return "AST_NODE_VAR";
      case AST_NODE_BLOCK: return "AST_NODE_BLOCK";
      case AST_NODE_RETURN: return "AST_NODE_RETURN";
      case AST_NODE_IF: return "AST_NODE_IF";
      case AST_NODE_WHILE: return "AST_NODE_WHILE";
      case AST_NODE_IMPORT: return "AST_NODE_IMPORT";
      case AST_NODE_VAR_DEF: return "AST_NODE_VAR_DEF";
      case AST_NODE_CONST_DEF: return "AST_NODE_CONST_DEF";
      case AST_NODE_FUNC_DEF: return "AST_NODE_FUNC_DEF";
      case AST_NODE_CALL: return "AST_NODE_CALL";
      default: IMPOSSIBLE(); return NULL;
    }
  }
}

static char* ast_node_lit_val2str(struct ASTNode const* node)
{
  if (node->type == AST_NODE_BOOLLIT) {
    if (node->data.lit.val == &OBJ_TRUE)
      return "TRUE";
    else
      return "FALSE";
  } else
    return node->data.lit.val->data.str.val;
}

static NoRet ast_node_print_(struct ASTNode const* node,
                             const uint16 indentation_level)
{
  if (!node) return;
  /* The array of spaces that represents the actual indentation */
  char indentation_per_level[SPACES_PER_LEVEL + 1];

  /* Fill the array */
  for (uint8 i                            = 0; i < SPACES_PER_LEVEL; ++i)
    indentation_per_level[i]              = SPACE_CH;
  indentation_per_level[SPACES_PER_LEVEL] = STR_END_CH;

  uint64 indentation_size = 0;
  char* indentation;
  indentation    = xmalloc(SPACES_PER_LEVEL * indentation_level * sizeof(char));
  indentation[0] = STR_END_CH;

  for (uint32 i = 0; i < indentation_level; ++i) {
    strcpy(&indentation[indentation_size], indentation_per_level);
    indentation_size += SPACES_PER_LEVEL;
  }

  printf("%s{\n", indentation);

  /* Print the type */
  char* type = ast_nodet2str(node->type);
  printf("%s%sType: %s\n", indentation_per_level, indentation, type);

  if (ast_node_is_lit(node)) {
    /* Print the value */
    char* val = ast_node_lit_val2str(node);
    printf("%s%sLiteral value: %s\n", indentation_per_level, indentation, val);
  } else if (ast_node_is_unop(node)) {
    printf("%s%sOperand:\n", indentation_per_level, indentation);
    ast_node_print_(node->data.unop.op, indentation_level + 1);
  } else if (ast_node_is_binop(node)) {
    printf("%s%sLeft operand:\n", indentation_per_level, indentation);
    ast_node_print_(node->data.binop.lop, indentation_level + 1);
    printf("%s%sRight operand:\n", indentation_per_level, indentation);
    ast_node_print_(node->data.binop.rop, indentation_level + 1);
  } else {
    switch (node->type) {
      case AST_NODE_VAR:
        printf("%s%sVariable name: %s\n",
               indentation_per_level,
               indentation,
               node->data.var.name->data.str.val);
        break;

      case AST_NODE_BLOCK:
        /* If block is not empty */
        if (node->data.block.stmts) {
          ast_nodes_print_(node->data.block.stmts, indentation_level + 1);
        }
        break;

      case AST_NODE_IF:
        /* Print the cond/body pairs */
        printf("%s%sCond/Body pairs:\n", indentation_per_level, indentation);
        printf("%s%s{\n", indentation_per_level, indentation);
        for (uint64 i = 0; i < node->data.if_.stmts->size; i += 2) {
          ast_node_print_(xvec_get(node->data.if_.stmts, i),
                          indentation_level + 2);
          ast_node_print_(xvec_get(node->data.if_.stmts, i + 1),
                          indentation_level + 2);
        }
        printf("%s%s}\n", indentation_per_level, indentation);

        /* Print the "else" body */
        if (node->data.if_.else_body) {
          printf("%s%s\"Else\" body:\n", indentation_per_level, indentation);
          ast_node_print_(node->data.if_.else_body, indentation_level + 1);
        }
        break;

      case AST_NODE_WHILE:
        /* Print the condition */
        printf("%s%sCondition:\n", indentation_per_level, indentation);
        ast_node_print_(node->data.while_.cond, indentation_level + 1);

        /* Print the body */
        printf("%s%sBody:\n", indentation_per_level, indentation);
        ast_node_print_(node->data.while_.body, indentation_level + 1);
        break;

      case AST_NODE_RETURN:
        printf("%s%sExpression:\n", indentation_per_level, indentation);
        ast_node_print_(node->data.ret.val, indentation_level + 1);
        break;

      case AST_NODE_IMPORT:
        printf("%s%sName: %s\n",
               indentation_per_level,
               indentation,
               node->data.import.val->data.str.val);
        break;

      case AST_NODE_FUNC_DEF:
        /* Print the name */
        printf("%s%sName: %s\n",
               indentation_per_level,
               indentation,
               node->data.func_def.name->data.str.val);

        /* Print the arguments */
        printf("%s%sArguments:\n", indentation_per_level, indentation);
        for (uint64 i = 0; i < node->data.func_def.args->size; ++i) {
          struct Object* obj;
          obj = xvec_get(node->data.func_def.args, i);
          printf("%s%s\t", indentation_per_level, indentation);
          obj_print(obj);
          printf("\n");
        }

        /* Print the body */
        printf("%s%sBody:\n", indentation_per_level, indentation);
        ast_node_print_(node->data.func_def.body, indentation_level + 1);
        break;

      case AST_NODE_VAR_DEF:
        /* Print the name */
        printf("%s%sName: %s\n",
               indentation_per_level,
               indentation,
               node->data.var_def.name->data.str.val);
        /* Print the initializator */
        if (node->data.var_def.initializator) {
          printf("%s%sInitializator:\n", indentation_per_level, indentation);
          ast_node_print_(node->data.var_def.initializator,
                          indentation_level + 1);
        }
        break;

      case AST_NODE_CONST_DEF:
        /* Print the name */
        printf("%s%sName: %s\n",
               indentation_per_level,
               indentation,
               node->data.const_def.name->data.str.val);
        /* Print the initializator */
        printf("%s%sInitializator:\n", indentation_per_level, indentation);
        ast_node_print_(node->data.const_def.initializator,
                        indentation_level + 1);
        break;

      case AST_NODE_ENUM_DEF:
        /* TODO: Implement */
        break;

      case AST_NODE_CALL:
        /* Print the name */
        printf("%s%sName: %s\n",
               indentation_per_level,
               indentation,
               node->data.call.name->data.str.val);

        /* Print the arguments */
        printf("%s%sArguments:\n", indentation_per_level, indentation);
        printf("%s%s{\n", indentation_per_level, indentation);
        ast_nodes_print_(node->data.call.args, indentation_level + 2);
        printf("%s%s}\n", indentation_per_level, indentation);
        break;

      default: IMPOSSIBLE(); return;
    }
  }

  /* Print the footer */
  printf("%s}\n", indentation);
  xfree(indentation);
}

NoRet ast_nodes_print_(struct XVector const* nodes,
                       const uint64 indentation_level)
{
  for (uint64 i = 0; i < nodes->size; ++i) {
    struct ASTNode* node = xvec_get(nodes, i);
    ast_node_print_(node, indentation_level);
  }
}

NoRet ast_node_print(struct ASTNode const* node)
{
  /* Obvious check */
  DASSERT(node, "");
  /* Do the actual print */
  ast_node_print_(node, 0);
}

NoRet ast_nodes_print(struct XVector const* nodes)
{
  ast_nodes_print_(nodes, 0);
}

NoRet ast_print_(struct AST const* ast, const uint64 indentation_level)
{
  /* Obvious check */
  DASSERT(ast, "");
  printf("AST Tree:\n");
  printf("{\n");
  ast_nodes_print_(&ast->childs, indentation_level + 1);
  printf("}\n");
}

NoRet ast_print(struct AST const* ast)
{
  /* Obvious check */
  DASSERT(ast, "");
  ast_print_(ast, 0);
}

#endif /* DEBUG */
