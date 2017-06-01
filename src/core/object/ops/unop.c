/* File: unop.c */
/* Creation Date: 2017-01-23 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* Object unary operations */

#include "core/object/ops/unop.h"

#include "core/object/object.h"
#include "core/object/inum/inum.h"
#include "core/object/dnum/dnum.h"
#include "core/object/string/string.h"

/********/
/* Main */
/********/

struct Object* obj_unop(
    const enum ObjectUnOpType type,
    struct Object const* obj)
{
    switch (obj->type) {
        case OBJT_INUM: return obj_inum_unop(type, obj);
        case OBJT_DNUM: return obj_dnum_unop(type, obj);
        case OBJT_STR: return obj_str_unop(type, obj);
        default:
            IMPOSSIBLE();
            return NULL;
    }
}
