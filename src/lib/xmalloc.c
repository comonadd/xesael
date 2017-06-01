/* File: xmalloc.c */
/* Creation Date: 2016-12-24*/
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */

#include "xmalloc.h"

#include <stdio.h>
#include <string.h>

#include "types.h"
#include "macros.h"
#include "xvec.h"

inline void* xmalloc(ByteSize size)
{
    return malloc(size);
}

inline void* xrealloc(void* ptr, ByteSize size)
{
    return realloc(ptr, size);
}

inline void* xcalloc(uint64 n, ByteSize size)
{
    return calloc(n, size);
}

inline NoRet xfree(void* ptr)
{
    free(ptr);
}
