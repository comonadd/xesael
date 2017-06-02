/* File: dnum.h */
/* Creation Date: 2017-01-09*/
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* DNum object */

#ifndef OBJ_DNUM_H
#define OBJ_DNUM_H

#include "lib/dbl.h"
#include "lib/types.h"

#include "core/object/object.h"
#include "core/object/ops/unop.h"

/***********/
/* Methods */
/***********/

struct Object* obj_dnum_new(const double val);

NoRet obj_dnum_delete(struct Object* obj);

struct Object* obj_dnum_new_from_str(char const* str);

bool obj_dnum_is_true(struct Object const* obj);

struct Object* obj_dnum_unop(const enum ObjectUnOpType type,
                             struct Object const* obj);

#endif /* OBJ_DNUM_H */
