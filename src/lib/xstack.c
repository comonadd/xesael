/* File: xstack.c */
/* Creation date: 17.12.2016 */
/* Author: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */

#include "xstack.h"

#include "types.h"
#include "macros.h"
#include "xmalloc.h"

static const uint8 ALLOC_SIZE = 64;

static NoRet xstack_grow(struct XStack* s)
{
    DASSERT(s, "");
    s->capacity += ALLOC_SIZE;
    s->elems = xrealloc(
        s->elems,
        s->capacity * PTR_SIZE);
}

NoRet xstack_init(struct XStack* s)
{
    DASSERT(s, "");
    s->elems = xmalloc(ALLOC_SIZE * PTR_SIZE);
    s->capacity = ALLOC_SIZE;
    s->size = 0;
}

NoRet xstack_deinit(struct XStack* s)
{
    DASSERT(s, "");
    xfree(s->elems);
}

void* xstack_pop(struct XStack* s)
{
    DASSERT(s, "");
    if (s->size == 0) return NULL;
    return s->elems[s->size--];
}

NoRet xstack_push(
    struct XStack* s,
    void* elem)
{
    DASSERT(s, "");
    DASSERT(elem, "");
    if (s->size >= s->capacity) xstack_grow(s);
    s->elems[++s->size] = elem;
}

void* xstack_top(struct XStack* s)
{
    DASSERT(s, "");
    if (s->size <= 0) return NULL;
    return s->elems[s->size];
}
