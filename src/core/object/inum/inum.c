/* File: inum.c */
/* Creation Date: 2017-01-09*/
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* INum object */
/* TODO: handle overflow (and if it exists, return an object of type BIGNUM) */

#include "core/object/inum/inum.h"

#include <math.h>

#include "log.h"
#include "err.h"

#include "lib/types.h"
#include "lib/macros.h"
#include "lib/xmalloc.h"
#include "lib/util.h"
#include "lib/str.h"

#include "core/object/object.h"
#include "core/object/bool/bool.h"
#include "core/object/ops/unop.h"

/***********/
/* Methods */
/***********/

struct Object* obj_inum_new(const int64 val)
{
    struct Object* obj = obj_new(OBJT_INUM);
    if (!obj) return NULL;
    obj->data.inum.val = val;
    LOG_DEBUG(
        "creating the INum object at address \"%p\" with value \"%ld\"",
        CAST(obj, void*),
        val);
    return obj;
}

NoRet obj_inum_delete(struct Object* obj)
{
    DASSERT(obj, "");
    LOG_DEBUG(
        "removing the INum object at address \"%p\" with value \"%ld\"",
        CAST(obj, void*),
        obj->data.inum.val);
    xfree(obj);
}

struct Object* obj_inum_new_from_str(
    const char* str)
{
    int64 val = str2int(str);
    return obj_inum_new(val);
}

bool obj_inum_is_true(
    const struct Object* obj)
{
    return obj->data.inum.val != 0;
}

struct Object* obj_inum_unop(
    const enum ObjectUnOpType type,
    const struct Object* obj)
{
    /* Obvious checks */
    OBJ_ASSERT_UNOPT_VALID(type);
    OBJ_ASSERT_TYPE(obj, OBJT_INUM);

    /* Retreive the value of a integer number object */
    int64 val = obj->data.inum.val;
    switch (type) {
        case O_UNOP_NEG: return obj_inum_new(-val);
        case O_UNOP_LNOT: return (val == 0) ?
            CAST(&OBJ_TRUE, struct Object*) :
            CAST(&OBJ_FALSE, struct Object*);
        case O_UNOP_BNOT: return obj_inum_new(~val);
        case O_UNOP_INCR: return obj_inum_new(++val);
        case O_UNOP_DECR: return obj_inum_new(--val);
        default:
            obj_err_set_unsupported_unop(type, obj->type);
            /* Fail */
            return NULL;
    }
}
