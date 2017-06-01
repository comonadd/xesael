/* File: nothing.h */
/* Creation Date: 2017-01-23 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* Nothing object */

#ifndef OBJ_NOTHING_H
#define OBJ_NOTHING_H

#include "lib/types.h"
#include "core/object/object.h"

extern const struct Object OBJ_NOTHING;

bool gsh_obj_is_nothing(const struct Object* obj);

#endif /* OBJ_NOTHING_H */
