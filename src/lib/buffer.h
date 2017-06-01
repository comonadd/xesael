/* File: buffer.h */
/* Creation Date: 2017-01-10*/
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* Generic buffer implementation */

#ifndef BUFFER_H
#define BUFFER_H

#include "types.h"

#define BUF_INITIAL_CAPACITY 1024
#define BUF_GROW_RATE 256

/**
   @len - length of a buffer
   @capacity - count of allocated bytes
   @val - the pointer to the actual data
**/
struct Buffer {
    uint64 len;
    uint64 capacity;
    char* val;
};

/***********/
/* Methods */
/***********/

bool buf_init(struct Buffer* buf);

NoRet buf_deinit(struct Buffer* buf);

NoRet buf_clear(struct Buffer* buf);

const char* buf_get_val(struct Buffer* buf);

bool buf_append_ch(
    struct Buffer* buf,
    const char ch);

bool buf_append(
    struct Buffer* buf,
    const char* str,
    const uint64 len);

bool buf_set(
    struct Buffer* buf,
    const char* str,
    const uint64 len);

#endif /* BUFFER_H */
