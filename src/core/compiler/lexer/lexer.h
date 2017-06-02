/* File: lexer.h */
/* Creation Date: 2016-12-30 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* The lexical analyser */

#ifndef LEXER_H
#define LEXER_H

#include <ctype.h>

#include "lib/macros.h"
#include "lib/str_stream.h"
#include "lib/types.h"
#include "lib/xvec.h"

/* This thing actually specifies what is the longest */
/* identifier length that we could lex */
#define LEXER_BUF_SIZE (1 << 16)

/**
   $ Description:
   #   This structure represents the state of the lexer
   @buf - buffer for storing temporary strings
   @stream - stream that lexer reads from
   @buf_len - length of a buffer
   @ln - current lexer's line number
   @toks - vector for storing results
**/
struct Lexer {
  char buf[LEXER_BUF_SIZE];
  struct StringStream stream;
  uint64 buf_len;
  LineNumber ln;
  struct XVector* toks;
};

/*************/
/* Constants */
/*************/

extern struct Lexer lexer;

/***********/
/* Methods */
/***********/

#define lexer_clear_buf()       \
  do {                          \
    lexer.buf[0]  = STR_END_CH; \
    lexer.buf_len = 0;          \
  } while (0)

#define lexer_save(ch)                       \
  do {                                       \
    lexer.buf[lexer.buf_len++] = ch;         \
    lexer.buf[lexer.buf_len]   = STR_END_CH; \
  } while (0)

#define lexer_get_buf_val() lexer.buf

#define lexer_get_stream() str_stream_get(&lexer.stream)
#define lexer_skip() str_stream_skip(&lexer.stream)
#define lexer_read() str_stream_read(&lexer.stream)

#define lexer_get() str_stream_get(&lexer.stream)
#define lexer_lookahead() str_stream_lookahead(&lexer.stream)

#define lexer_eq(ch) (lexer_get() == (ch))
#define lexer_lookahead_eq(ch) (lexer_lookahead() == (ch))

#define lexer_at_dot() lexer_eq(DOT_CH)
#define lexer_at_xdigit() isxdigit(lexer_get())
#define lexer_at_numlit() (lexer_at_dot() || lexer_at_xdigit())

#define lexer_at_newline() ch_isnewline(lexer_get())
#define lexer_at_eof() (lexer_eq(STR_END_CH))
#define lexer_at_id() (isalnum(lexer_get()) || lexer_eq(UNDERSCORE_CH))

#define lexer_cc_should_be(___ch) DASSERT(lexer_eq((___ch)), "")

#define ch_can_start_numlit(___ch) (((___ch) == DOT_CH) || isdigit((___ch)))
#define ch_can_start_id(___ch) (isalpha((___ch)) || (___ch == UNDERSCORE_CH))

#define lexer_get_toks_count() lexer.toks->size
#define lexer_get_ln() lexer.ln
#define lexer_incr_ln() ++lexer.ln
#define lexer_get_pt() \
  CAST(lexer.toks->elems[lexer.toks->size - 1], struct Token*)

#define lexer_init_buf() lexer_clear_buf()
#define lexer_init_stream() str_stream_init(&lexer.stream, text);
#define lexer_init(___text)            \
  do {                                 \
    lexer_init_buf();                  \
    lexer_init_stream();               \
    lexer.toks = xvec_new(tok_free_f); \
    lexer.ln   = 1;                    \
  } while (0)

#define lexer_deinit_buf() lexer_clear_buf()

#define lexer_deinit()  \
  do {                  \
    lexer_deinit_buf(); \
  } while (0)

#define lexer_append_tok(___tok) \
  xvec_append(lexer.toks, CAST((___tok), struct Token*))

bool lexer_append_tok_keyword(char const* val);

enum TokenType ch_single2tt(const char ch);

/**************/
/* Benchmarks */
/**************/

#if BENCH
NoRet bench_lexer(void);
#endif /* BENCH */

/*********/
/* Tests */
/*********/

#if TEST
NoRet test_lexer(void);
#endif /* TEST */

#endif /* LEXER_H */
