/* File: lex.c */
/* Creation date: 2017-02-04 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* lex() function */

#include "core/compiler/lexer/lex/lex.h"

#include "err.h"

#include "lib/xvec.h"
#include "lib/util.h"

#include "core/compiler/token/token.h"
#include "core/compiler/lexer/lexer.h"
#include "core/compiler/compiler.h"
#include "core/compiler/lexer/lex/lex_strlit.h"
#include "core/compiler/lexer/lex/lex_numlit.h"
#include "core/compiler/lexer/lex/lex_identifier.h"
#include "core/compiler/lexer/lex/lex_comment.h"

static NoRet lex_plus_ch(void)
{
    /* Obvious check */
    lexer_cc_should_be(PLUS_CH);
    lexer_skip();
    switch (lexer_get()) {
        case PLUS_CH:
            lexer_append_tok(&RTOKS[TOK_INCR]);
            lexer_skip(); break;
        case EQ_CH:
            lexer_append_tok(&RTOKS[TOK_ASSIGN_ADD]);
            lexer_skip(); break;
        default:
            if (tok_is_binop(lexer_get_pt()) ||
                tok_is_sep(lexer_get_pt()) ||
                tok_is_invalid(lexer_get_pt())) {
                /* Don't do anything (we don't need unary '+') */
            } else lexer_append_tok(&RTOKS[TOK_ADD]);
    }
}

static NoRet lex_minus_ch(void)
{
    /* Obvious check */
    lexer_cc_should_be(MINUS_CH);
    lexer_skip();
    switch (lexer_get()) {
        case MINUS_CH:
            lexer_append_tok(&RTOKS[TOK_DECR]);
            lexer_skip(); break;
        case EQ_CH:
            lexer_append_tok(&RTOKS[TOK_ASSIGN_SUB]);
            lexer_skip(); break;
        default:
            if (tok_is_binop(lexer_get_pt()) ||
                tok_is_sep(lexer_get_pt()) ||
                tok_is_invalid(lexer_get_pt())) {
                lexer_append_tok(&RTOKS[TOK_NEG]);
            } else lexer_append_tok(&RTOKS[TOK_SUB]);
    }
}

/**
   $ Description:
   #   This function reads an character
   #   and if it sees that this character
   #   is equal to ch1, it appends a reserved
   #   token of type type1, otherwise
   #   of type type_else
**/
static NoRet read_rtok1(
    const char ch1,
    const enum TokenType type1,
    const enum TokenType type_else)
{
    lexer_skip();
    if (lexer_get() == ch1) {
        lexer_append_tok(&RTOKS[type1]);
        lexer_skip();
    } else lexer_append_tok(&RTOKS[type_else]);
}

struct XVector* lex(char const* text)
{
    if (!text) {
        /* Say that all is ok */
        err_reset();
        return NULL;
    }

    /* Initialize the lexer */
    lexer_init(text);

    while (true) {
        /* Retreive the current character */
        char ch = lexer_get();

        /* If character is a space */
        if (ch_isspace(ch)) goto skip;

        /* If character is a backslash */
        if (ch == BACKSLASH_CH) goto skip;

        /* If character is a newline */
        if (ch_isnewline(ch)) {
            /* Increment line number and append a newline token */
            lexer_incr_ln();
            lexer_append_tok(&RTOKS[TOK_SEP_NEWLINE]);
            goto skip;
        }

        /* This block of code checks whenether the */
        /* current character is "single" */
        /* (which means that there is no possibility that it represents something another) */
        /* and appends the reserved token into the array of tokens depending on the character */
        {
            /* Retreive the token type that character represents */
            const enum TokenType type = ch_single2tt(ch);
            /* If it is a single */
            if (type != TOK_INVALID) {
                /* Append and skip it */
                lexer_append_tok(&RTOKS[type]);
                goto skip;
            }
        }

        /* If character could possibly start a number literal */
        if (ch_can_start_numlit(ch)) {
            if (!lex_numlit()) goto out;
            continue;
        }

        /* If character could possible start an identifier */
        if (ch_can_start_id(ch)) {
            lex_identifier();
            continue;
        }

        switch (ch) {
            case PLUS_CH: lex_plus_ch(); break;
            case MINUS_CH: lex_minus_ch(); break;

            case STAR_CH:
                if (lexer_lookahead_eq(STAR_CH)) {
                    lexer_skip();
                    read_rtok1(EQ_CH, TOK_ASSIGN_POW, TOK_POW);
                } else read_rtok1(EQ_CH, TOK_ASSIGN_MUL, TOK_MUL);
                break;

            case SLASH_CH: read_rtok1(EQ_CH, TOK_ASSIGN_DIV, TOK_DIV); break;
            case MODULUS_CH: read_rtok1(EQ_CH, TOK_ASSIGN_MOD, TOK_MOD); break;
            case EQ_CH: read_rtok1(EQ_CH, TOK_EQ, TOK_ASSIGN); break;
            case EXCLAM_CH: read_rtok1(EQ_CH, TOK_NEQ, TOK_LNOT); break;

            case LT_CH:
                if (lexer_lookahead_eq(LT_CH)) {
                    lexer_skip();
                    read_rtok1(EQ_CH, TOK_ASSIGN_LS, TOK_LS);
                } else read_rtok1(EQ_CH, TOK_LTE, TOK_LT);
                break;

            case GT_CH:
                if (lexer_lookahead_eq(GT_CH)) {
                    lexer_skip();
                    read_rtok1(EQ_CH, TOK_ASSIGN_RS, TOK_RS);
                } else read_rtok1(EQ_CH, TOK_LTE, TOK_GT);
                break;

            case PIPE_CH:
                if (lexer_lookahead_eq(PIPE_CH)) {
                    lexer_skip();
                    read_rtok1(EQ_CH, TOK_ASSIGN_LOR, TOK_LOR);
                } else read_rtok1(EQ_CH, TOK_ASSIGN_BOR, TOK_BOR);
                break;

            case AMP_CH:
                if (lexer_lookahead_eq(AMP_CH)) {
                    lexer_skip();
                    read_rtok1(EQ_CH, TOK_ASSIGN_LAND, TOK_LAND);
                } else read_rtok1(EQ_CH, TOK_ASSIGN_BAND, TOK_BAND);
                break;

            case CARET_CH: read_rtok1(EQ_CH, TOK_ASSIGN_BXOR, TOK_BXOR); break;
            case HASH_CH: if (!lex_comment()) goto out; continue;
            case QUOTE_CH: if (!lex_strlit()) goto out; continue;
            case STR_END_CH: case EOF_CH: goto done;
            default: goto out_undefined_token;
        }

    skip:
        lexer_skip();
    }

done:
    lexer_deinit();
    return lexer.toks;

out_undefined_token:
    compiler_err_set(lexer_get_ln(), "undefined token");
    goto out;

out:
    lexer_deinit();
    xvec_delete(lexer.toks);
    return NULL;
}
