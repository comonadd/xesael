/* File: compile.c */
/* Creation date: 2017-02-04 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* compile() and compile_file() functions */

#include "core/compiler/compile.h"

#include "core/compiler/ast/ast.h"
#include "core/compiler/ast/print.h"
#include "core/compiler/compiler.h"
#include "core/compiler/lexer/lex/lex.h"
#include "core/compiler/parser/parse/parse.h"
#include "core/compiler/token/token.h"
#include "core/compiler/translator/translate/translate.h"
#include "core/vm/bytecode/bytecode.h"
#include "err.h"
#include "platform/xfile.h"

/********/
/* Main */
/********/

struct ByteCode* compile(char const* const src_name, char const* const text)
{
  /* Initialize the compiler state */
  compiler_prepare(src_name);

  /* Lex */
  struct XVector* toks = lex(text);
  if (!toks) goto out;

#if DEBUG
  printf("Tokens:\n");
  printf("{\n");
  for (uint64 i = 0; i < toks->size; ++i) {
    printf("\t%s\n", tok2str(xvec_get(toks, i)));
  }
  printf("}\n");
#endif /* DEBUG */

  /* Parse */
  struct AST* ast = parse(toks);
  if (!ast) {
    xvec_delete(toks);
    goto out;
  }

#if DEBUG
  ast_print(ast);
#endif /* DEBUG */

  struct ByteCode* bc;
  translate(&bc, ast);
  if (!bc) {
    xvec_delete(toks);
    ast_delete(ast);
    goto out;
  }

#if DEBUG
  bytecode_print(bc);
#endif /* DEBUG */

  /* Succeed */
  return bc;

out:
  /* Fail */
  return false;
}

struct ByteCode* compile_file(char const* path)
{
  XFile f;
  char* content;
  ByteSize size;

  /* Open file */
  if (!xfile_open(&f, path, XFM_R, false, false)) return NULL;

  /* Load file */
  if (!xfile_load(&f, &content, &size)) return NULL;

  /* Close file */
  if (!xfile_close(&f)) return NULL;

  /* Succceed */
  return compile(path, content);
}
