/* File: ast_node.h */
/* Creation Date: 2017-01-20 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* Abstract syntax tree node implementation */

#ifndef AST_NODE_H
#define AST_NODE_H

#include "core/compiler/token/token.h"
#include "core/vm/register.h"
#include "lib/types.h"

/* NOTE: Order reserved */
/* NOTE: The change of order requires the change of the */
/*       constants below */
enum ASTNodeType {
  /* Literals */
  AST_NODE_INUMLIT,
  AST_NODE_DNUMLIT,
  AST_NODE_BOOLLIT,
  AST_NODE_STRLIT,

  /* Variable */
  AST_NODE_VAR,

  /* Unary prefix operators */
  AST_NODE_NEG,
  AST_NODE_LNOT,
  AST_NODE_BNOT,
  AST_NODE_LEN,

  /* Unary postfix operators */
  AST_NODE_INCR,
  AST_NODE_DECR,

  /* Binary arithmetic operators */
  AST_NODE_ADD,
  AST_NODE_SUB,
  AST_NODE_MUL,
  AST_NODE_DIV,
  AST_NODE_MOD,
  AST_NODE_POW,
  AST_NODE_LAND,
  AST_NODE_LOR,
  AST_NODE_BAND,
  AST_NODE_BOR,
  AST_NODE_BXOR,
  AST_NODE_LS,
  AST_NODE_RS,

  /* Binary relational operators */
  AST_NODE_EQ,
  AST_NODE_NEQ,
  AST_NODE_LT,
  AST_NODE_GT,
  AST_NODE_LTE,
  AST_NODE_GTE,

  /* Binary assign operator */
  AST_NODE_ASSIGN,

  AST_NODE_BLOCK,
  AST_NODE_IF,
  AST_NODE_WHILE,

  AST_NODE_RETURN,
  AST_NODE_CONTINUE,
  AST_NODE_BREAK,
  AST_NODE_IMPORT,
  AST_NODE_VAR_DEF,
  AST_NODE_CONST_DEF,
  AST_NODE_ENUM_DEF,
  AST_NODE_FUNC_DEF,
  AST_NODE_CALL
};

#define AST_NODE_LIT_FIRST AST_NODE_INUMLIT
#define AST_NODE_LIT_LAST AST_NODE_STRLIT

#define AST_NODE_UNOP_FIRST AST_NODE_NEG
#define AST_NODE_UNOP_LAST AST_NODE_DECR

#define AST_NODE_BINOP_FIRST AST_NODE_ADD
#define AST_NODE_BINOP_LAST AST_NODE_ASSIGN

#define AST_NODE_COMP_STMT_FIRST AST_NODE_BLOCK
#define AST_NODE_COMP_STMT_LAST AST_NODE_WHILE

union ASTNodeData {
  /* Literal */
  struct {
    struct Object* val;
  } lit;

  /* Variable */
  struct {
    struct Object* name;
  } var;

  /* Unary operator */
  struct {
    struct ASTNode* op;
  } unop;

  /* Binary operator */
  struct {
    struct ASTNode* lop;
    struct ASTNode* rop;
  } binop;

  /* BLOCK statement */
  struct {
    struct XVector* stmts;
  } block;

  /* IF statement */
  struct {
    struct ASTNode* else_body;
    struct XVector* stmts;
  } if_;

  /* WHILE statement */
  struct {
    struct ASTNode* cond;
    struct ASTNode* body;
  } while_;

  /* Return */
  struct {
    struct ASTNode* val;
  } ret;

  /* IMPORT statement */
  struct {
    struct Object* val;
  } import;

  /* Function defintion */
  struct {
    struct Object* name;
    struct XVector* args;
    struct ASTNode* body;
  } func_def;

  /* Variable definition */
  struct {
    struct Object* name;
    struct ASTNode* initializator;
  } var_def;

  /* Constant definition */
  struct {
    struct Object* name;
    struct ASTNode* initializator;
  } const_def;

  /* Enum definition */
  struct {
    struct Object* name;
    struct XVector* items;
  } enum_def;

  /* Call */
  struct {
    struct Object* name;
    struct XVector* args;
  } call;
};

struct ASTNode {
  union ASTNodeData data;
  LineNumber ln;
  RegisterID rid;
  enum ASTNodeType type;
};

/***********/
/* Helpers */
/***********/

#define ast_nodet_is_lit(__type) \
  (((__type) >= AST_NODE_LIT_FIRST) && ((__type) <= AST_NODE_LIT_LAST))
#define ast_node_is_lit(__node) ast_nodet_is_lit((__node)->type)

#define ast_nodet_is_unop(__type) \
  (((__type) >= AST_NODE_UNOP_FIRST) && ((__type) <= AST_NODE_UNOP_LAST))
#define ast_node_is_unop(__node) ast_nodet_is_unop((__node)->type)

#define ast_nodet_is_binop(__type) \
  (((__type) >= AST_NODE_BINOP_FIRST) && ((__type) <= AST_NODE_BINOP_LAST))
#define ast_node_is_binop(__node) ast_nodet_is_binop((__node)->type)

#define ast_nodet_is_comp_stmt(__type)       \
  (((__type) >= AST_NODE_COMP_STMT_FIRST) && \
   ((__type) <= AST_NODE_COMP_STMT_LAST))
#define ast_node_is_comp_stmt(__node) ast_nodet_is_comp((__node)->type)

/***********/
/* Getters */
/***********/

#define ast_node_func_def_name(___node) (___node)->data.func_def.name

/***********/
/* Methods */
/***********/

struct ASTNode* ast_node_new(const enum ASTNodeType type, const LineNumber ln);

NoRet ast_node_delete(struct ASTNode* node);

NoRet ast_node_free_f(void* node);

struct ASTNode* ast_node_dup(struct ASTNode* node);

enum ASTNodeType tt_unop2astt(const enum TokenType type);
#define tok_unop2astt(tok) tt_unop2astt(tok->type)

#define tok_binop2astt(tok) tt_binop2astt(tok->type)
enum ASTNodeType tt_binop2astt(const enum TokenType type);

#define tok_assign_shorthand2astt(tok) tt_assign_shorthand2astt(tok->type)
enum ASTNodeType tt_assign_shorthand2astt(const enum TokenType type);

struct Object* ast_node_lit_get_obj(struct ASTNode* node);

/****************/
/* Constructors */
/****************/

struct ASTNode* mk_ast_node_lit(const enum ASTNodeType type,
                                const LineNumber ln,
                                struct Object* val);

struct ASTNode* mk_ast_node_var(const LineNumber ln, struct Object* name);

struct ASTNode* mk_ast_node_unop(const LineNumber ln,
                                 const enum ASTNodeType type,
                                 struct ASTNode* op);

struct ASTNode* mk_ast_node_binop(const LineNumber ln,
                                  const enum ASTNodeType type,
                                  struct ASTNode* lop,
                                  struct ASTNode* rop);

struct ASTNode* mk_ast_node_block(const LineNumber ln, struct XVector* stmts);

struct ASTNode* mk_ast_node_if(const LineNumber ln,
                               struct ASTNode* else_body,
                               struct XVector* stmts);

struct ASTNode* mk_ast_node_while(const LineNumber ln,
                                  struct ASTNode* cond,
                                  struct ASTNode* body);

struct ASTNode* mk_ast_node_return(const LineNumber ln, struct ASTNode* val);

struct ASTNode* mk_ast_node_import(const LineNumber ln, struct Object* name);

struct ASTNode* mk_ast_node_func_def(const LineNumber ln,
                                     struct ASTNode* body,
                                     struct Object* name,
                                     struct XVector* args);

struct ASTNode* mk_ast_node_var_def(const LineNumber ln,
                                    struct Object* name,
                                    struct ASTNode* initializator);

struct ASTNode* mk_ast_node_const_def(const LineNumber ln,
                                      struct Object* name,
                                      struct ASTNode* initializator);

struct ASTNode* mk_ast_node_call(const LineNumber ln,
                                 struct Object* name,
                                 struct XVector* args);

#endif /* AST_NODE_H */
