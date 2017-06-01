/* File: nothing.c */
/* Creation Date: 2017-01-23 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* Nothing object */

#include "core/object/nothing/nothing.h"

#include "lib/types.h"

#include "core/object/object.h"

const struct Object OBJ_NOTHING = {
    .ref_count = 1
};

bool gsh_obj_is_nothing(const struct Object* obj)
{
    return obj == &OBJ_NOTHING;
}
