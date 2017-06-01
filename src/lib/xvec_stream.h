/* File: xvec_stream.h */
/* Creation Date: 2017-01-02*/
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */

#ifndef XVEC_STREAM_H
#define XVEC_STREAM_H

#include "lib/types.h"
#include "lib/xvec.h"

/**
   @pos - current position in the vector
   @vec - the actual vector
   @end - thing that will be returned
   #      when we reached the end of
   #      stream
**/
struct XVectorStream {
    uint64 pos;
    struct XVector const* vec;
    void const* end;
};

NoRet xvec_stream_init(
    struct XVectorStream* stream,
    struct XVector const* const vec,
    void const* const end);

void const* xvec_stream_read(
    struct XVectorStream* const stream);

void const* xvec_stream_get(
    struct XVectorStream* const stream);

NoRet xvec_stream_skip(
    struct XVectorStream* const stream);

void const* xvec_stream_lookahead(
    struct XVectorStream* const stream);

void const* xvec_stream_lookback(
    struct XVectorStream* const stream);

#if TEST
NoRet xvec_stream_test(void);
#endif /* TEST */

#endif /* XVEC_STREAM_H */
