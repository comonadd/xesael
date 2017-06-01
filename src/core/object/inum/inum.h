/* File: inum.h */
/* Creation Date: 2017-01-09*/
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* INum object */

#ifndef OBJ_INUM_H
#define OBJ_INUM_H

#include "lib/types.h"

#include "core/object/object.h"
#include "core/object/ops/unop.h"

/***********/
/* Methods */
/***********/

struct Object* obj_inum_new(const int64 val);

NoRet obj_inum_delete(struct Object* obj);

struct Object* obj_inum_new_from_str(
    const char* str);

bool obj_inum_is_true(
    const struct Object* obj);

struct Object* obj_inum_unop(
    const enum ObjectUnOpType type,
    const struct Object* obj);

#endif /* OBJ_INUM_H */
