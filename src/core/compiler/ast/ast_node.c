/* File: ast_node.c */
/* Creation Date: 2017-01-20 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* Abstract syntax tree node implementation */

#include "core/compiler/ast/ast_node.h"

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/xmalloc.h"
#include "lib/xvec.h"

#include "lib/types.h"

#include "core/object/bool/bool.h"
#include "core/object/dnum/dnum.h"
#include "core/object/inum/inum.h"
#include "core/object/object.h"

#include "core/compiler/ast/ast.h"
#include "core/compiler/ast/print.h"

/***********/
/* Helpers */
/***********/

struct ASTNode* ast_node_new(const enum ASTNodeType type, const LineNumber ln)
{
  struct ASTNode* node = xmalloc(sizeof(*node));
  node->ln             = ln;
  node->rid            = 0;
  node->type           = type;
  return node;
}

NoRet ast_node_delete(struct ASTNode* node)
{
  /* Obvious check */
  if (!node) return;

  if (ast_node_is_lit(node)) {
    obj_decr_ref_count(node->data.lit.val);
  } else if (node->type == AST_NODE_VAR) {
    obj_decr_ref_count(node->data.var.name);
  } else if (ast_node_is_unop(node)) {
    ast_node_delete(node->data.unop.op);
  } else if (ast_node_is_binop(node)) {
    ast_node_delete(node->data.binop.lop);
    ast_node_delete(node->data.binop.rop);
  } else {
    switch (node->type) {
      case AST_NODE_BLOCK:
        if (node->data.block.stmts) xvec_delete(node->data.block.stmts);
        break;

      case AST_NODE_IF:
        DASSERT(node->data.if_.stmts,
                "if should always have at least ONE cond/body pair");
        xvec_delete(node->data.if_.stmts);
        if (node->data.if_.else_body) ast_node_delete(node->data.if_.else_body);
        break;

      case AST_NODE_WHILE:
        ast_node_delete(node->data.while_.cond);
        ast_node_delete(node->data.while_.body);
        break;

      case AST_NODE_RETURN: ast_node_delete(node->data.ret.val); break;

      case AST_NODE_IMPORT: obj_decr_ref_count(node->data.import.val); break;

      case AST_NODE_FUNC_DEF:
        obj_decr_ref_count(node->data.func_def.name);
        obj_xvec_delete(node->data.func_def.args);
        break;

      case AST_NODE_VAR_DEF:
        obj_decr_ref_count(node->data.var_def.name);
        ast_node_delete(node->data.var_def.initializator);
        break;

      case AST_NODE_CONST_DEF:
        obj_decr_ref_count(node->data.const_def.name);
        ast_node_delete(node->data.const_def.initializator);
        break;

      case AST_NODE_ENUM_DEF:
        obj_decr_ref_count(node->data.enum_def.name);
        xvec_delete(node->data.enum_def.items);
        break;

      case AST_NODE_CALL:
        obj_decr_ref_count(node->data.call.name);
        xvec_delete(node->data.call.args);
        break;

      default: PASS();
    }
  }

  xfree(node);
}

NoRet ast_node_free_f(void* node)
{
  ast_node_delete(CAST(node, struct ASTNode*));
}

struct ASTNode* ast_node_dup(struct ASTNode* node)
{
  struct ASTNode* new_node = xmalloc(sizeof(*new_node));
  memcpy(new_node, node, sizeof(*new_node));
  return new_node;
}

enum ASTNodeType tt_unop2astt(const enum TokenType type)
{
  /* Obvious check */
  DASSERT(tt_is_unop(type), "");
  return CAST(AST_NODE_UNOP_FIRST + type - TOK_UNOP_FIRST, enum ASTNodeType);
}

enum ASTNodeType tt_binop2astt(const enum TokenType type)
{
  /* Obvious check */
  DASSERT(tt_is_binop(type), "");
  if (tt_is_assign(type)) return AST_NODE_ASSIGN;
  return CAST(AST_NODE_BINOP_FIRST + type - TOK_BINOP_FIRST, enum ASTNodeType);
}

enum ASTNodeType tt_assign_shorthand2astt(const enum TokenType type)
{
  /* Obvious check */
  DASSERT(tt_is_assign_shorthand(type), "");
  return CAST(AST_NODE_BINOP_FIRST + type - TOK_ASSIGN_SHORTHAND_FIRST,
              enum ASTNodeType);
}

struct Object* ast_node_lit_get_obj(struct ASTNode* node)
{
  struct Object* node_val = node->data.lit.val;

  switch (node->type) {
    case AST_NODE_STRLIT: return node_val;
    case AST_NODE_INUMLIT: return obj_inum_new_from_str(node_val->data.str.val);
    case AST_NODE_DNUMLIT: return obj_dnum_new_from_str(node_val->data.str.val);
    default:
      IMPOSSIBLE();
      /* Fail */
      return false;
  }
}

/****************/
/* Constructors */
/****************/

struct ASTNode* mk_ast_node_lit(const enum ASTNodeType type,
                                const LineNumber ln,
                                struct Object* val)
{
  DASSERT(val, "");
  DASSERT(ast_nodet_is_lit(type), "");
  struct ASTNode* node = ast_node_new(type, ln);
  node->data.lit.val   = val;
  obj_incr_ref_count(val);
  return node;
}

struct ASTNode* mk_ast_node_var(const LineNumber ln, struct Object* name)
{
  DASSERT(name, "");
  struct ASTNode* node    = ast_node_new(AST_NODE_VAR, ln);
  node->data.var_def.name = name;
  obj_incr_ref_count(name);
  return node;
}

struct ASTNode* mk_ast_node_unop(const LineNumber ln,
                                 const enum ASTNodeType type,
                                 struct ASTNode* op)
{
  DASSERT(op, "given operand is NULL");
  DASSERT(ast_nodet_is_unop(type), "");
  struct ASTNode* node = ast_node_new(type, ln);
  node->data.unop.op   = op;
  return node;
}

struct ASTNode* mk_ast_node_binop(const LineNumber ln,
                                  const enum ASTNodeType type,
                                  struct ASTNode* lop,
                                  struct ASTNode* rop)
{
  /* Obvious checks */
  DASSERT(lop, "left operand is NULL");
  DASSERT(rop, "right operand is NULL");
  DASSERT(ast_nodet_is_binop(type), "");
  struct ASTNode* node = ast_node_new(type, ln);
  node->data.binop.lop = lop;
  node->data.binop.rop = rop;
  return node;
}

struct ASTNode* mk_ast_node_block(const LineNumber ln, struct XVector* stmts)
{
  struct ASTNode* node   = ast_node_new(AST_NODE_BLOCK, ln);
  node->data.block.stmts = stmts;
  return node;
}

struct ASTNode* mk_ast_node_if(const LineNumber ln,
                               struct ASTNode* else_body,
                               struct XVector* stmts)
{
  struct ASTNode* node     = ast_node_new(AST_NODE_IF, ln);
  node->data.if_.else_body = else_body;
  node->data.if_.stmts     = stmts;
  return node;
}

struct ASTNode* mk_ast_node_while(const LineNumber ln,
                                  struct ASTNode* cond,
                                  struct ASTNode* body)
{
  struct ASTNode* node   = ast_node_new(AST_NODE_WHILE, ln);
  node->data.while_.cond = cond;
  node->data.while_.body = body;
  return node;
}

struct ASTNode* mk_ast_node_return(const LineNumber ln, struct ASTNode* val)
{
  struct ASTNode* node = ast_node_new(AST_NODE_RETURN, ln);
  node->data.ret.val   = val;
  return node;
}

struct ASTNode* mk_ast_node_import(const LineNumber ln, struct Object* name)
{
  struct ASTNode* node  = ast_node_new(AST_NODE_IMPORT, ln);
  node->data.import.val = name;
  obj_incr_ref_count(name);
  return node;
}

struct ASTNode* mk_ast_node_func_def(const LineNumber ln,
                                     struct ASTNode* body,
                                     struct Object* name,
                                     struct XVector* args)
{
  struct ASTNode* node     = ast_node_new(AST_NODE_FUNC_DEF, ln);
  node->data.func_def.name = name;
  node->data.func_def.args = args;
  node->data.func_def.body = body;
  obj_incr_ref_count(name);
  return node;
}

struct ASTNode* mk_ast_node_var_def(const LineNumber ln,
                                    struct Object* name,
                                    struct ASTNode* initializator)
{
  DASSERT(name, "");

  struct ASTNode* node             = ast_node_new(AST_NODE_VAR_DEF, ln);
  node->data.var_def.name          = name;
  node->data.var_def.initializator = initializator;
  obj_incr_ref_count(name);
  return node;
}

struct ASTNode* mk_ast_node_const_def(const LineNumber ln,
                                      struct Object* name,
                                      struct ASTNode* initializator)
{
  DASSERT(name, "");
  DASSERT(initializator, "");

  struct ASTNode* node               = ast_node_new(AST_NODE_CONST_DEF, ln);
  node->data.const_def.name          = name;
  node->data.const_def.initializator = initializator;
  obj_incr_ref_count(name);
  return node;
}

struct ASTNode* mk_ast_node_call(const LineNumber ln,
                                 struct Object* name,
                                 struct XVector* args)
{
  struct ASTNode* node = ast_node_new(AST_NODE_CALL, ln);
  node->data.call.name = name;
  node->data.call.args = args;
  obj_incr_ref_count(name);
  return node;
}
