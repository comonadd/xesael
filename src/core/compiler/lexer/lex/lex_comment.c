/* File: lex_comment.c */
/* Creation date: 2017-02-04 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* lex_comment() function */

#include "core/compiler/lexer/lex/lex_comment.h"

#include "lib/util.h"

#include "core/compiler/compiler.h"
#include "core/compiler/token/token.h"
#include "core/compiler/lexer/lexer.h"

bool lex_comment(void)
{
    /* Obvious check */
    lexer_cc_should_be(HASH_CH);

    /* Save the line number that comment was on */
    const LineNumber ln = lexer_get_ln();

    /* Skip the hash '#' symbol */
    lexer_skip();

    /* If next character is a opening brace '{' */
    if (lexer_eq(LBRACE_CH)) {
        /* Multi-line #{ comment }# */

        /* Skip the opening brace '{' */
        lexer_skip();
        while (true) {
            /* If we at the end of the multi-line comment */
            if (lexer_eq(RBRACE_CH) && lexer_lookahead_eq(HASH_CH)) {
                /* Skip '}' */
                lexer_skip();
                /* Skip '#' */
                lexer_skip();
                /* Exit from the loop */
                break;
            }

            /* If we at the newline character */
            if (lexer_at_newline()) {
                /* Append NEWLINE token */
                lexer_append_tok(&RTOKS[TOK_SEP_NEWLINE]);
                /* Increment line number */
                lexer_incr_ln();
            } else if (lexer_get() == STR_END_CH) {
                /* Set the appropriate error message */
                compiler_err_set(
                    ln, "unclosed multi-line comment");
                /* Fail */
                return false;
            }

            /* Skip */
            lexer_skip();
        }
    } else {
        /* Single-line # comment */
        while (true) {
            /* If we at the newline character */
            if (lexer_at_newline()) {
                /* Append NEWLINE token */
                lexer_append_tok(&RTOKS[TOK_SEP_NEWLINE]);
                /* Increment the line number */
                lexer_incr_ln();
                break;
            }

            /* If we reached the end of the file */
            if (lexer_at_eof()) break;

            /* Skip */
            lexer_skip();
        }
    }

    /* Succeed */
    return true;
}
