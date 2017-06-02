/* File: lexer.c */
/* Creation Date: 2016-12-11 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* The lexical analyser */

#include "core/compiler/lexer/lexer.h"

#include "err.h"

#include "lib/macros.h"
#include "lib/str_stream.h"
#include "lib/types.h"
#include "lib/util.h"
#include "lib/xmalloc.h"

#include "platform/path.h"
#include "platform/xfile.h"

#include "core/compiler/lexer/lex/lex.h"
#include "core/compiler/token/token.h"
#include "core/object/object.h"

/*************/
/* Constants */
/*************/

struct Lexer lexer;

/***********/
/* Methods */
/***********/

bool lexer_append_tok_keyword(char const* val)
{
  enum TokenType i = TOK_KEYWORD_FIRST;

  while (i <= TOK_KEYWORD_LAST) {
    if (STREQ(val, TOKS_STR[i])) {
      /* Found such a keyword */
      lexer_append_tok(&RTOKS[i]);
      /* Succeed */
      return true;
    }
    ++i;
  }

  /* Fail */
  return false;
}

enum TokenType ch_single2tt(const char ch)
{
  switch (ch) {
    case SEMI_CH: return TOK_SEP_SEMI;
    case COLON_CH: return TOK_SEP_COLON;
    case COMMA_CH: return TOK_SEP_COMMA;
    case LBRACKET_CH: return TOK_SEP_LBRACKET;
    case RBRACKET_CH: return TOK_SEP_RBRACKET;
    case LPAREN_CH: return TOK_SEP_LPAREN;
    case RPAREN_CH: return TOK_SEP_RPAREN;
    case LBRACE_CH: return TOK_SEP_LBRACE;
    case RBRACE_CH: return TOK_SEP_RBRACE;
    case DOLLAR_CH: return TOK_LEN;
    case AT_CH: return TOK_IN;
    case TILDA_CH: return TOK_BNOT;
    default: return TOK_INVALID;
  }
}

/**************/
/* Benchmarks */
/**************/

#if BENCH
#include <sys/time.h>
#include <unistd.h>

#define ITER_COUNT CAST(1 << 18, uint64)

#define BENCH_LEXER_BENCHMARK_BEG()                    \
  struct timeval tval_before, tval_after, tval_result; \
  gettimeofday(&tval_before, NULL);

#define BENCH_LEXER_BENCHMARK_END(name, ...)         \
  gettimeofday(&tval_after, NULL);                   \
  timersub(&tval_after, &tval_before, &tval_result); \
  printf("\t[\"");                                   \
  printf(name, __VA_ARGS__);                         \
  printf("\"]: %ld.%06ld\n",                         \
         (long int)tval_result.tv_sec,               \
         (long int)tval_result.tv_usec);

NoRet bench_lex_all_separately(void)
{
  struct XVector* toks;
  BENCH_LEXER_BENCHMARK_BEG();
  for (uint64 i = 0; i < ITER_COUNT; ++i) {
    for (uint64 j = 1; j < (RTOKS_COUNT - 1); ++j) {
      toks = lex(TOKS_STR[j]);
#if DEBUG
      assert(toks);
#endif /* DEBUG */
      tok_xvec_delete(toks);
    }

    toks = lex("\"something\"");
    tok_xvec_delete(toks);
    toks = lex("123532");
    tok_xvec_delete(toks);
    toks = lex("somethingqwe");
    tok_xvec_delete(toks);
  }
  BENCH_LEXER_BENCHMARK_END("lex all tokens, each separately, %u time(s)",
                            ITER_COUNT);
}

NoRet bench_lex_all_together(void)
{
  char* all_tokens;
  all_tokens = str_arr_concat(&TOKS_STR[1], RTOKS_COUNT - 1);
  BENCH_LEXER_BENCHMARK_BEG();
  for (uint64 i = 0; i < ITER_COUNT; ++i) {
    struct XVector* toks;
    toks = lex(all_tokens);
    tok_xvec_delete(toks);
    toks = lex("\"something\"");
    tok_xvec_delete(toks);
    toks = lex("123532");
    tok_xvec_delete(toks);
    toks = lex("somethingqwe");
    tok_xvec_delete(toks);
  }
  BENCH_LEXER_BENCHMARK_END("lex all tokens, together, %u time(s)", ITER_COUNT);
  xfree(all_tokens);
}

NoRet bench_lexer(void)
{
  bench_lex_all_separately();
  bench_lex_all_together();
}

#endif /* BENCH */

/*********/
/* Tests */
/*********/

#if TEST
#include <assert.h>

NoRet TEST_LEX_CONDITION(const char const* name,
                         const char const* path,
                         const bool cond)
{
  err_reset();
  XFile file;
  char* file_contents;
  ByteSize file_contents_size;
  struct XVector* toks;

  char* path_ = path_concat(LEXER_TESTS_RESOURCES_DIR, path);
  assert(xfile_open(&file, path_, XFM_R, false, false));
  xfree(path_);

  assert(xfile_load(&file, &file_contents, &file_contents_size));

  toks = lex(file_contents);
  if (cond)
    assert(err_is_ok());
  else
    assert(!(toks || err_is_ok()));
  if (toks) xvec_delete(toks);
  assert(xfile_close(&file));

  xfree(file_contents);
  err_reset();
  printf("\t[\"%s\"]: passed\n", name);
}

NoRet test_lexer(void)
{
  TEST_LEX_CONDITION("eof", "eof.gshl", true);

  TEST_LEX_CONDITION("bad string literals", "bad/strlit.gshl", false);
  TEST_LEX_CONDITION("good string literals", "good/strlit.gshl", true);

  TEST_LEX_CONDITION("bad number literals", "bad/numlit.gshl", false);
  TEST_LEX_CONDITION("good number literals", "good/numlit.gshl", true);
}

#endif /* TEST */
