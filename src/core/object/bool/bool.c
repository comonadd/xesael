/* File: bool.c */
/* Creation Date: 2017-01-23 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* Boolean object */

#include "core/object/bool/bool.h"

#include "core/object/dnum/dnum.h"
#include "core/object/inum/inum.h"
#include "core/object/nothing/nothing.h"
#include "core/object/object.h"
#include "core/object/string/string.h"
#include "lib/macros.h"
#include "lib/types.h"

const struct Object OBJ_TRUE = {.ref_count = 1};
const struct Object OBJ_FALSE = {.ref_count = 1};

bool obj_is_true(const struct Object* obj)
{
  /* Obvious check */
  DASSERT(obj, "");
  if (obj == &OBJ_TRUE) return true;
  if ((obj == &OBJ_FALSE) || gsh_obj_is_nothing(obj)) return false;
  switch (obj->type) {
    case OBJT_STR: return obj_str_is_true(obj);
    case OBJT_INUM: return obj_inum_is_true(obj);
    case OBJT_DNUM: return obj_dnum_is_true(obj);
    default: IMPOSSIBLE(); return false;
  }
}

bool obj_is_false(const struct Object* obj)
{
  /* Obvious check */
  DASSERT(obj, "");
  return !obj_is_true(obj);
}
