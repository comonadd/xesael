/* File: buffer.c */
/* Creation Date: 2017-01-10*/
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* Generic buffer implementation */

#include "buffer.h"

#include <stdlib.h>

#include "macros.h"
#include "types.h"
#include "xmalloc.h"

/***********/
/* Helpers */
/***********/

static bool buf_grow(struct Buffer* buf)
{
  buf->capacity += BUF_GROW_RATE;
  buf->val = xrealloc(buf->val, buf->capacity * sizeof(*buf->val));
  if (!buf->val) return false;
  return true;
}

/***********/
/* Methods */
/***********/

bool buf_init(struct Buffer* buf)
{
  buf->capacity = BUF_INITIAL_CAPACITY;
  buf->val      = xmalloc(BUF_INITIAL_CAPACITY * sizeof(*buf->val));
  if (!buf->val) return false;
  buf->val[0] = STR_END_CH;
  buf->len    = 0;
  return true;
}

NoRet buf_deinit(struct Buffer* buf)
{
  xfree(buf->val);
}

NoRet buf_clear(struct Buffer* buf)
{
  buf->val[0] = STR_END_CH;
  buf->len    = 0;
}

const char* buf_get_val(struct Buffer* buf)
{
  return buf->val;
}

bool buf_append_ch(struct Buffer* buf, const char ch)
{
  ++buf->len;
  if (buf->len >= buf->capacity)
    if (!buf_grow(buf)) return false;
  buf->val[buf->len - 1] = ch;
  buf->val[buf->len]     = STR_END_CH;
  return true;
}

bool buf_append(struct Buffer* buf, const char* str, const uint64 len)
{
  /* If there is no space for the new string */
  if ((buf->len + len) >= buf->capacity) {
    buf->capacity = buf->len + len + BUF_GROW_RATE;
    buf->val      = xrealloc(buf->val, sizeof(*buf->val) * buf->capacity);
    if (!buf->val) return false;
  }

  /* Actually append the string */
  memcpy(&buf->val[buf->len], str, len);
  buf->len += len;
  buf->val[buf->len] = STR_END_CH;

  return true;
}

bool buf_set(struct Buffer* buf, const char* str, const uint64 len)
{
  buf->len = len;

  if (buf->len >= buf->capacity) {
    xfree(buf->val);
    buf->val = xmalloc(buf->capacity * sizeof(*buf->val));
    if (!buf->val) return false;
  }

  memcpy(buf->val, str, len);
  buf->val[len] = STR_END_CH;

  return true;
}
