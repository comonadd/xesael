/* File: dnum.c */
/* Creation Date: 2017-01-09*/
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* DNum object */
/* TODO: handle overflow (and if it exists, return an object of type BIGNUM) */

#include "core/object/dnum/dnum.h"

#include "log.h"
#include "err.h"

#include "lib/types.h"
#include "lib/macros.h"
#include "lib/xmalloc.h"
#include "lib/util.h"
#include "lib/dbl.h"
#include "lib/str.h"

#include "core/object/object.h"
#include "core/object/bool/bool.h"
#include "core/object/ops/unop.h"

/***********/
/* Methods */
/***********/

struct Object* obj_dnum_new(const double val)
{
    struct Object* obj = obj_new(OBJT_DNUM);
    if (!obj) return NULL;
    obj->data.dnum.val = val;
    LOG_DEBUG(
        "creating the DNum object at address \"%p\" with value \"%lf\"",
        CAST(obj, void*),
        val);
    return obj;
}

NoRet obj_dnum_delete(struct Object* obj)
{
    DASSERT(obj, "");
    LOG_DEBUG(
        "removing the DNum object at address \"%p\" with value \"%lf\"",
        CAST(obj, void*),
        obj->data.dnum.val);
    xfree(obj);
}

struct Object* obj_dnum_new_from_str(
    const char* str)
{
    double val = str2double(str);
    return obj_dnum_new(val);
}

bool obj_dnum_is_true(
    const struct Object* obj)
{
    return dbl_neq(obj->data.dnum.val, 0.0);
}

struct Object* obj_dnum_unop(
    const enum ObjectUnOpType type,
    const struct Object* obj)
{
    double val = obj->data.dnum.val;
    switch (type) {
        case O_UNOP_NEG: return obj_dnum_new(-val);
        case O_UNOP_LNOT:
            return (dbl_eq(val, 0.0)) ?
                CAST(&OBJ_FALSE, struct Object*) :
            CAST(&OBJ_TRUE, struct Object*);
        case O_UNOP_INCR: return obj_dnum_new(++val);
        case O_UNOP_DECR: return obj_dnum_new(--val);
        default:
            obj_err_set_unsupported_unop(type, obj->type);
            return NULL;
    }
}
