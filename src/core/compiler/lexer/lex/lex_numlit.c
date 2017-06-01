/* File: lex_numlit.c */
/* Creation date: 2017-02-04 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* lex_numlit() function */

#include "core/compiler/lexer/lex/lex_numlit.h"

#include "lib/types.h"

#include "core/compiler/token/token.h"
#include "core/compiler/compiler.h"
#include "core/compiler/lexer/lexer.h"

bool lex_numlit(void)
{
    bool has_dot = false;

    /* Retreive the number literal value */
    char ch;
    do {
        /* Read the next character */
        ch = lexer_read();
        if (ch == DOT_CH) {
            /* If we already got the dot */
            if (has_dot) {
                /* We got an malformed number literal */
                goto out_malformed_numlit;
            }
            has_dot = true;
        }
        /* Save character to the buffer */
        lexer_save(ch);
    } while (lexer_at_numlit());

    /* Create new number literal token and append it */
    char* val = lexer_get_buf_val();
    if (has_dot) lexer_append_tok(tok_dnumlit_new(val));
    else lexer_append_tok(tok_inumlit_new(val));
    lexer_clear_buf();
    return true;

out_malformed_numlit:
    compiler_err_set(lexer_get_ln(), "malformed number literal");
    return false;
}
