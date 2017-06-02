/* File: str_stream.h */
/* Creation Date: 2017-01-02 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* The stream of characters */

#ifndef STR_STREAM_H
#define STR_STREAM_H

#include "types.h"

struct StringStream {
  uint64 len;
  const char* data;
};

NoRet str_stream_init(struct StringStream* const stream, const char* data);

char str_stream_read(struct StringStream* const stream);

NoRet str_stream_skip(struct StringStream* const stream);

char str_stream_get(struct StringStream* const stream);

char str_stream_lookahead(struct StringStream* const stream);

char str_stream_lookback(struct StringStream* const stream);

#endif /* STR_STREAM_H */
