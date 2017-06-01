/* File: immediate.c */
/* Creation Date: 2017-01-16*/
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* This thing is used in the bytecode as the representation */
/* of literals */

#include "core/vm/immediate/immediate.h"

#include "err.h"
#include "lib/types.h"
#include "lib/macros.h"
#include "lib/xmalloc.h"
#include "core/object/object.h"

struct Immediate* immediate_new(
    struct Object* obj)
{
    /* Obvious check */
    DASSERT(obj, "");
    struct Immediate* immediate;
    immediate = xmalloc(sizeof(*immediate));
    if (!immediate) {
        err_set_not_enough_memory();
        return NULL;
    }
    immediate->id = 0;
    immediate->obj = obj;
    return immediate;
}

NoRet immediate_delete(
    struct Immediate* immediate)
{
    obj_decr_ref_count(immediate->obj);
    xfree(immediate);
}

NoRet immediate_free_f(void* ptr)
{
    immediate_delete(ptr);
}
