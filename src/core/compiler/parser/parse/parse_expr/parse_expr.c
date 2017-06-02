/* File: parse_expr.c */
/* Creation Date: 2017-01-18 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* parse_expr() function */

#include "core/compiler/parser/parse/parse_expr/parse_expr.h"

#include "err.h"

#include "lib/types.h"
#include "lib/xstack.h"
#include "lib/xvec_stream.h"

#include "core/compiler/ast/ast.h"
#include "core/compiler/parser/parse/parse_expr/parse_atom.h"
#include "core/compiler/parser/parser.h"
#include "core/compiler/token/preced.h"
#include "core/compiler/token/token.h"

struct ASTNode* parse_expr_(const TokenPrecedence min_prec)
{
  /* Retreive the first "atom" in the expression */
  struct ASTNode* res = parse_atom();
  if (!res) return NULL;

  /* Get the current token */
  struct Token const* tok = parser_get();

  /* The precendance of the token */
  TokenPrecedence preced;

  /* While the current token is a binary operator and it's precision */
  /* is greater-than or equal to the given minimum */
  while (tok_is_binop(tok) && (preced = tok_preced(tok)) >= min_prec) {
    /* Skip the binary operator */
    parser_skip();

    /* Right-hand side token */
    struct ASTNode* rhs;

    if (tok_is_left_assoc(tok)) {
      /* Operator is left-associative */
      rhs = parse_expr_(preced + 1);
    } else {
      /* Operator is right-associative */
      rhs = parse_expr_(preced);
    }

    /* If failed to parse the right-hand side of an expression */
    if (!rhs) {
      /* Deallocate the left-hand side expression */
      ast_node_delete(res);
      return NULL;
    }

    /* If the binary operator is an assign shorthand */
    /* ('+=', '-=') */
    if (tok_is_assign_shorthand(tok)) {
      /* Retreive the operation */
      enum ASTNodeType binop_type;
      binop_type = tt_assign_shorthand2astt(tok->type);

      /* Unfold it ('a += b' = 'a = a + b') */
      res = mk_ast_node_binop(
        parser_get_ln(),
        AST_NODE_ASSIGN,
        res,
        mk_ast_node_binop(parser_get_ln(), binop_type, ast_node_dup(res), rhs));
    } else {
      res =
        mk_ast_node_binop(parser_get_ln(), tt_binop2astt(tok->type), res, rhs);
    }

    /* Retreive the current token */
    tok = parser_get();
  }

  /* Succeed */
  return res;
}

struct ASTNode* parse_expr(void)
{
  return parse_expr_(0);
}
