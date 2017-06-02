/* File: parse_func_def_stmt.c */
/* Creation Date: 2017-01-30 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* parse_func_def_stmt() function */

#include "core/compiler/parser/parse/parse_stmt/parse_def_stmt/parse_func_def_stmt.h"

#include "core/compiler/compiler.h"
#include "core/compiler/parser/parse/parse_stmt/parse_stmt.h"
#include "core/compiler/parser/parser.h"
#include "core/compiler/token/token.h"
#include "core/object/object.h"

/* funcdef_args_list = LPAREN var {var,} RPAREN; */
static struct XVector* __parse_args(void)
{
  /* Obvious check */
  tok_assert_type(parser_get(), TOK_SEP_LPAREN);

  /* Save the current line number */
  const LineNumber ln = parser_get_ln();

  /* Skip opening paren '(' */
  if (!parser_eat(TOK_SEP_LPAREN)) return NULL;

  /* Create a new vector for the arguments */
  struct XVector* args = xvec_new(obj_free_f);
  struct Token const* tok;
  while (true) {
    tok = parser_read();
    switch (tok->type) {
      case TOK_ID:
        /* We got an function argument name */
        xvec_append(args, tok->val);
        break;
      case TOK_SEP_COMMA:
        /* We got an function argument names */
        /* separator ',' */
        continue;
      case TOK_SEP_RPAREN:
        /* We reached the end of a function */
        /* arguments list */
        goto done;
      default:
        /* We got something that we     */
        /* probably won't to see in the */
        /* function arguments list      */
        goto out_invalid_syntax_free_args;
    }
  }

done:
  /* Succeed */
  return args;

out_invalid_syntax_free_args:
  compiler_err_set(ln, "invalid syntax");
  goto out_free_args;

out_free_args:
  xvec_delete(args);
  /* Fail */
  return NULL;
}

/* funcdef_stmt = FUNCTION_KEYWORD identifier funcdef_args_list stmt_block; */
struct ASTNode* parse_func_def_stmt(void)
{
  /* Obvious check */
  tok_assert_type(parser_get(), TOK_KEYWORD_FUNC);

  /* Save the line number that function was defined on */
  const LineNumber ln = parser_get_ln();

  /* Skip FUNCTION_KEYWORD token */
  parser_skip();

  /* Retreive the name object of a function */
  struct Token const* func_name_tok = parser_eat(TOK_ID);
  if (!func_name_tok) return NULL;
  struct Object* name_obj = func_name_tok->val;

  /* Parse the arguments of a function */
  /* function_arguments_list: (ID [, ID]) */
  struct XVector* args = __parse_args();
  if (!args) return NULL;

  /* Parse the body of a function */
  struct ASTNode* body = parse_stmt();
  if (!body) goto out_free_args;

  /* Create a new FUNCTION node */
  return mk_ast_node_func_def(ln, body, name_obj, args);

out_free_args:
  /* Delete the allocated argments vector */
  xvec_delete(args);
  /* Fail */
  return NULL;
}
