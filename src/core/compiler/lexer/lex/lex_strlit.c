/* File: lex_strlit.c */
/* Creation date: 2017-02-04 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* lex_strlit() function */

#include "core/compiler/lexer/lex/lex_strlit.h"

#include "lib/types.h"

#include "core/compiler/token/token.h"
#include "core/compiler/compiler.h"
#include "core/compiler/lexer/lexer.h"

bool lex_strlit(void)
{
    /* Obvious check */
    lexer_cc_should_be(QUOTE_CH);

    /* Save the line number (for error reporting) */
    LineNumber ln = lexer_get_ln();

    /* Skip the opening quote */
    lexer_skip();

    /* Retreiving value of string literal */
    char ch;
    while ((ch = lexer_read()) != QUOTE_CH) {
        /* Get the character */
        switch (ch) {
            case '\\':
                switch (lexer_get()) {
                    case 'a': ch = '\a'; break;
                    case 'b': ch = '\b'; break;
                    case 'f': ch = '\v'; break;
                    case 'n': ch = '\n'; break;
                    case 'r': ch = '\r'; break;
                    case 't': ch = '\t'; break;
                    case 'v': ch = '\v'; break;
                    default: goto out_invalid_esc;
                }
                break;
            case STR_END_CH:
                goto out_unclosed_strlit;
            default: PASS();
        }
        lexer_save(ch);
    }

    lexer_append_tok(tok_strlit_new(lexer_get_buf_val()));
    lexer_clear_buf();
    return true;

out_unclosed_strlit:
    compiler_err_set(ln, "unclosed string literal");
    goto out;

out_invalid_esc:
    compiler_err_set(ln, "invalid escape sequence");
    goto out;

out:
    lexer_clear_buf();
    return false;
}
