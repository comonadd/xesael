/* File: bool.h */
/* Creation Date: 2017-01-23 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* Boolean object */

#ifndef GSH_OBJ_BOOL_H
#define GSH_OBJ_BOOL_H

#include "core/object/object.h"

extern const struct Object OBJ_TRUE;
extern const struct Object OBJ_FALSE;

bool obj_is_true(const struct Object* obj);
bool obj_is_false(const struct Object* obj);

#endif /* GSH_OBJ_BOOL_H */
