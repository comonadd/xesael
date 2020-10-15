/* File: xvec_stream.c */
/* Creation Date: 2017-01-02*/
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */

#include "xvec_stream.h"

#include "macros.h"
#include "types.h"
#include "xmalloc.h"
#include "xvec.h"

NoRet xvec_stream_init(struct XVectorStream* stream,
                       struct XVector const* const vec,
                       void const* const end)
{
  stream->pos = 0;
  stream->vec = vec;
  stream->end = end;
}

void const* xvec_stream_read(struct XVectorStream* const stream)
{
  if (stream->pos >= stream->vec->size) return stream->end;
  ++stream->pos;
  return xvec_get(stream->vec, stream->pos - 1);
}

NoRet xvec_stream_skip(struct XVectorStream* const stream)
{
  ++stream->pos;
}

void const* xvec_stream_get(struct XVectorStream* const stream)
{
  if (stream->pos >= stream->vec->size) return stream->end;
  return xvec_get(stream->vec, stream->pos);
}

void const* xvec_stream_lookahead(struct XVectorStream* const stream)
{
  if (stream->pos + 1 >= stream->vec->size) return stream->end;
  return xvec_get(stream->vec, stream->pos + 1);
}

void const* xvec_stream_lookback(struct XVectorStream* const stream)
{
  if (stream->pos == 0) return stream->end;
  return xvec_get(stream->vec, stream->pos - 1);
}

#if XVEC_STREAM_TEST
#include <assert.h>

static NoRet test_xvec_stream_read(void)
{
  int* a = xmalloc(sizeof(*a));
  int* b = xmalloc(sizeof(*b));
  *a     = 4;
  *b     = 3;

  struct XVector vec;
  xvec_init(&vec, NULL);
  xvec_append(&vec, a);
  xvec_append(&vec, b);
  struct XVectorStream s;
  xvec_stream_init(&s, &vec, NULL);

  int read = 0;
  read     = *(int*)xvec_stream_read(&s);
  assert(read == *a);
  read = *(int*)xvec_stream_read(&s);
  assert(read == *b);
  assert(!xvec_stream_read(&s));
  assert(!xvec_stream_read(&s));
  assert(!xvec_stream_read(&s));
  xvec_deinit(&vec);
}

NoRet xvec_stream_test(void)
{
  printf("XVector stream testing:\n");
  test_xvec_stream_read();
  printf("\t[\"test_xvec_stream_read\"]: passed\n");
}

#endif /* XVEC_STREAM_TEST */
