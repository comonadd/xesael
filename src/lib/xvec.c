/* File: xvec.c */
/* Creation date: 15.12.2016 */
/* Author: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* Generic vector implementation */

#include "xvec.h"

#include "types.h"
#include "macros.h"
#include "xmalloc.h"

static const uint8 ALLOC_SIZE = 64;

static bool xvec_grow(struct XVector* v)
{
    DASSERT(v, "v is NULL");

    v->capacity += ALLOC_SIZE;
    v->elems = xrealloc(v->elems,
                        v->capacity * PTR_SIZE);
    if (!v->elems) return false;
    return true;
}

bool xvec_init(
    struct XVector* v,
    NoRet (*elem_free_f)(void* elem))
{
    DASSERT(v, "v is NULL");
    v->size = 0;
    v->capacity = ALLOC_SIZE;
    v->elems = xmalloc(v->capacity * sizeof(*v->elems));

    if (elem_free_f) {
        v->elem_free_f = elem_free_f;
    } else {
        v->elem_free_f = xfree;
    }

    if (!v->elems) return false;
    return true;
}

struct XVector* xvec_new(NoRet (*elem_free_f)(void* elem))
{
    struct XVector* v = xmalloc(sizeof(*v));
    if (!v) return NULL;
    xvec_init(v, elem_free_f);
    return v;
}

NoRet xvec_deinit(struct XVector* v)
{
    DASSERT(v, "v is NULL");
    while (v->size > 0) {
        v->elem_free_f(v->elems[v->size - 1]);
        --v->size;
    }
    xfree(v->elems);
}

NoRet xvec_delete(struct XVector* v)
{
    DASSERT(v, "v is NULL");
    xvec_deinit(v);
    xfree(v);
}

bool xvec_set(
    struct XVector* v,
    const uint64 index,
    void* item)
{
    DASSERT(v, "v is NULL");

    /* If there is no space for given index */
    if (index >= v->capacity) {
        v->capacity = index + ALLOC_SIZE;
        v->elems = xrealloc(
            v->elems,
            sizeof(*v->elems) * v->capacity);
        if (!v->elems) return false;
    }

    if (index >= v->size) {
        v->size = index + 1;
    }

    v->elems[index] = item;
    return true;
}

NoRet xvec_append(
    struct XVector* v,
    void* item)
{
    DASSERT(v, "v is NULL");
    DASSERT(item, "given item is NULL");

    if (v->size >= v->capacity) xvec_grow(v);
    v->elems[v->size++] = item;
}

void* xvec_get(
    const struct XVector* v,
    const uint64 index)
{
    DASSERT(v, "v is NULL");
    DASSERT(index < v->size, "index out of range");

    if (index >= v->size) return NULL;
    return v->elems[index];
}

struct XVector* xvec_dup(const struct XVector* v)
{
    /* Obvious check */
    DASSERT(v, "tried to duplicate NULL vector");

    /* Allocate a new vector */
    struct XVector* dup = xmalloc(sizeof(*dup));

    /* Copy the data */
    dup->size = v->size;
    dup->capacity = v->capacity;
    dup->elems = xmalloc(v->capacity * sizeof(*dup->elems));
    for (uint64 i = 0; i < v->size; ++i)
        dup->elems[i] = v->elems[i];
    return dup;
}
