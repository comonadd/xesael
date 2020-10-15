/* File: xmalloc.h */
/* Creation Date: 2016-12-24*/
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* The allocator for debugging */
/* If flag DEBUG_MEMORY is set when compiling, */
/* it will print all the info about all allocations. */
/* If not, it will use the standard functions */

#ifndef XMALLOC_H
#define XMALLOC_H

#include "types.h"

extern void* xmalloc(ByteSize size);
extern void* xrealloc(void* ptr, ByteSize size);
extern void* xcalloc(uint64 n, ByteSize size);
extern NoRet xfree(void* ptr);

#endif /* XMALLOC_H */
