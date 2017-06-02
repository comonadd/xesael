/* File: lex_identifier.c */
/* Creation date: 2017-02-04 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* lex_identifier() function */

#include "core/compiler/lexer/lex/lex_identifier.h"

#include "lib/macros.h"
#include "lib/types.h"

#include "core/compiler/lexer/lexer.h"
#include "core/compiler/token/token.h"

bool lex_identifier(void)
{
  /* Obvious check */
  DASSERT(lexer_at_id(), "");

  /* Retreive the identifier value */
  do {
    lexer_save(lexer_read());
  } while (lexer_at_id());

  char* val = lexer_get_buf_val();
  /* If this is a keyword */
  if (lexer_append_tok_keyword(val)) goto done;
  lexer_append_tok(tok_id_new(val));

done:
  lexer_clear_buf();
  return true;
}
