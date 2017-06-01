/* File: token.c */
/* Creation date: 11.12.2016 */
/* Author: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* Helpers for working with tokens */

#include "core/compiler/token/token.h"

#include "lib/types.h"
#include "lib/macros.h"
#include "lib/xmalloc.h"
#include "log.h"
#include "core/object/string/string.h"

/*******************/
/* Reserved tokens */
/*******************/

const struct Token RTOKS[RTOKS_COUNT] = {
    /* Invalid token */
    {NULL, TOK_INVALID},

    /* Separators */
    {NULL, TOK_SEP_NEWLINE},
    {NULL, TOK_SEP_SEMI},
    {NULL, TOK_SEP_COLON},
    {NULL, TOK_SEP_COMMA},
    {NULL, TOK_SEP_LBRACKET},
    {NULL, TOK_SEP_RBRACKET},
    {NULL, TOK_SEP_LPAREN},
    {NULL, TOK_SEP_RPAREN},
    {NULL, TOK_SEP_LBRACE},
    {NULL, TOK_SEP_RBRACE},

    /* Unary prefix operators */
    {NULL, TOK_NEG},
    {NULL, TOK_LNOT},
    {NULL, TOK_BNOT},
    {NULL, TOK_LEN},

    /* Unary postfix operators */
    {NULL, TOK_INCR},
    {NULL, TOK_DECR},

    /* Binary arithmetic operators */
    {NULL, TOK_ADD},
    {NULL, TOK_SUB},
    {NULL, TOK_MUL},
    {NULL, TOK_DIV},
    {NULL, TOK_MOD},
    {NULL, TOK_POW},
    {NULL, TOK_BAND},
    {NULL, TOK_BOR},
    {NULL, TOK_BXOR},
    {NULL, TOK_LS},
    {NULL, TOK_RS},

    /* Binary boolean operators */
    {NULL, TOK_LAND},
    {NULL, TOK_LOR},

    /* Binary relational operators */
    {NULL, TOK_EQ},
    {NULL, TOK_NEQ},
    {NULL, TOK_LT},
    {NULL, TOK_GT},
    {NULL, TOK_LTE},
    {NULL, TOK_GTE},
    {NULL, TOK_IN},

    /* Assign operator */
    {NULL, TOK_ASSIGN},

    /* Arithmetic assign operator shorthands */
    {NULL, TOK_ASSIGN_ADD},
    {NULL, TOK_ASSIGN_SUB},
    {NULL, TOK_ASSIGN_MUL},
    {NULL, TOK_ASSIGN_DIV},
    {NULL, TOK_ASSIGN_MOD},
    {NULL, TOK_ASSIGN_POW},
    {NULL, TOK_ASSIGN_BAND},
    {NULL, TOK_ASSIGN_BOR},
    {NULL, TOK_ASSIGN_BXOR},
    {NULL, TOK_ASSIGN_LS},
    {NULL, TOK_ASSIGN_RS},

    /* Boolean assign opeartor shorthands */
    {NULL, TOK_ASSIGN_LAND},
    {NULL, TOK_ASSIGN_LOR},

    /* "define" keywords */
    {NULL, TOK_KEYWORD_VAR},
    {NULL, TOK_KEYWORD_CONST},
    {NULL, TOK_KEYWORD_ENUM},
    {NULL, TOK_KEYWORD_FUNC},

    /* "flow" keywords */
    {NULL, TOK_KEYWORD_CONTINUE},
    {NULL, TOK_KEYWORD_BREAK},
    {NULL, TOK_KEYWORD_RETURN},
    {NULL, TOK_KEYWORD_IF},
    {NULL, TOK_KEYWORD_ELIF},
    {NULL, TOK_KEYWORD_ELSE},

    /* "loop" keywords */
    {NULL, TOK_KEYWORD_LOOP},
    {NULL, TOK_KEYWORD_FOR},
    {NULL, TOK_KEYWORD_WHILE},

    /* Keywords */
    {NULL, TOK_KEYWORD_TRUE},
    {NULL, TOK_KEYWORD_FALSE},
    {NULL, TOK_KEYWORD_IMPORT}
};

/***********************************/
/* String representation of tokens */
/***********************************/

const char* TOKS_STR[TOKS_COUNT] = {
    /* Invalid token */
    "invalid token",

    /* Separators */
    "\\n",
    ";",
    ":",
    ",",
    "[",
    "]",
    "(",
    ")",
    "{",
    "}",

    /* Unary prefix operators */
    "-",
    "!",
    "~",
    "$",

    /* Unary postfix operators */
    "++",
    "--",

    /* Binary arithmetic operators */
    "+",
    "-",
    "*",
    "/",
    "%",
    "**",
    "&",
    "|",
    "^",
    "<<",
    ">>",

    /* Binary boolean operators */
    "&&",
    "||",

    /* Binary relational operators */
    "==",
    "!=",
    "<",
    ">",
    "<=",
    ">=",
    "@",

    /* Assign operator */
    "=",

    /* Arithmetic assign operator shorthands */
    "+=",
    "-=",
    "*=",
    "/=",
    "%=",
    "**=",
    "&=",
    "|=",
    "^=",
    "<<=",
    ">>=",

    /* Boolean assign operator shorthands */
    "&&=",
    "||=",

    /* "define" keywords */
    "var",
    "const",
    "enum",
    "function",

    /* "flow" keywords */
    "continue",
    "break",
    "return",
    "if",
    "elif",
    "else",

    /* "loop" keywords */
    "loop",
    "for",
    "while",

    /* Keywords */
    "true",
    "false",
    "import",

    /* Not reserved */
    "integer number literal",
    "double-precision number literal",
    "string literal",
    "identifier"
};

/****************/
/* "Is" helpers */
/****************/

bool tt_is_left_assoc(const enum TokenType type)
{
    if (tt_is_prefix_unop(type)) return false;
    if (tt_is_assign(type)) return false;
    return true;
}

/***********/
/* Helpers */
/***********/

NoRet tok_free_f(void* tok)
{
    struct Token* tok_ = CAST(tok, struct Token*);
    if (!tok_is_reserved(tok_)) {
        /* All not reserved tokens have value */
        obj_decr_ref_count(tok_->val);
        /* Deallocate the actual token */
        xfree(tok);
    }
}

bool tok_is_stmt_sep(struct Token const* tok)
{
    return
        (tok->type == TOK_SEP_NEWLINE) ||
        (tok->type == TOK_SEP_SEMI);
}

bool tok_can_start_def_stmt(struct Token const* tok)
{
    return
        (tok->type == TOK_KEYWORD_VAR) ||
        (tok->type == TOK_KEYWORD_CONST) ||
        (tok->type == TOK_KEYWORD_ENUM) ||
        (tok->type == TOK_KEYWORD_FUNC);
}

bool tok_can_start_flow_stmt(struct Token const* tok)
{
    return
        (tok->type == TOK_KEYWORD_BREAK) ||
        (tok->type == TOK_KEYWORD_CONTINUE) ||
        (tok->type == TOK_KEYWORD_RETURN);
}

bool tok_can_start_loop_stmt(struct Token const* tok)
{
    return
        (tok->type == TOK_KEYWORD_FOR) ||
        (tok->type == TOK_KEYWORD_WHILE) ||
        (tok->type == TOK_KEYWORD_LOOP);
}

/*****************/
/* Debug helpers */
/*****************/

#if DEBUG
NoRet tok_xvec_print(const struct XVector* vec)
{
    for (uint64 i = 0; i < vec->size; ++i) {
        struct Token const* tok = xvec_get(vec, i);
        printf("%lu: %s\n", i, tok2str(tok));
    }
}
#endif /* DEBUG */

/****************/
/* Constructors */
/****************/

static struct Token* tok_new(const enum TokenType type)
{
    struct Token* tok = xmalloc(sizeof(*tok));
    tok->type = type;
    return tok;
}

struct Token* tok_inumlit_new(const char* val)
{
    struct Token* tok = tok_new(TOK_INUMLIT);
    tok->val = obj_str_new(val);
    return tok;
}

struct Token* tok_dnumlit_new(const char* val)
{
    struct Token* tok = tok_new(TOK_DNUMLIT);
    tok->val = obj_str_new(val);
    return tok;
}

struct Token* tok_strlit_new(const char* val)
{
    struct Token* tok = tok_new(TOK_STRLIT);
    tok->val = obj_str_new(val);
    return tok;
}

struct Token* tok_id_new(const char* val)
{
    struct Token* tok = tok_new(TOK_ID);
    tok->val = obj_str_new(val);
    return tok;
}
