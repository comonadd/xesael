/* File: str_stream.c */
/* Creation Date: 2017-01-02 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* The stream of characters */

#include "str_stream.h"

#include <string.h>

#include "macros.h"
#include "types.h"
#include "xmalloc.h"

NoRet str_stream_init(struct StringStream* const stream, const char* data)
{
  stream->len  = strlen(data);
  stream->data = data;
}

char str_stream_read(struct StringStream* const stream)
{
  if (!stream->len) return STR_END_CH;
  --stream->len;
  ++stream->data;
  return *(stream->data - 1);
}

NoRet str_stream_skip(struct StringStream* const stream)
{
  if (stream->len) {
    --stream->len;
    ++stream->data;
  }
}

char str_stream_get(struct StringStream* const stream)
{
  return *stream->data;
}

char str_stream_lookahead(struct StringStream* const stream)
{
  return *(stream->data + 1);
}

char str_stream_lookback(struct StringStream* const stream)
{
  return *(stream->data - 1);
}
