/* File: object.c */
/* Creation Date: 2017-01-06*/
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* The object */

#include "core/object/object.h"

#include "log.h"

#include "lib/dict.h"
#include "lib/macros.h"
#include "lib/types.h"
#include "lib/util.h"
#include "lib/xmalloc.h"

#include "core/object/bool/bool.h"
#include "core/object/dnum/dnum.h"
#include "core/object/func/func.h"
#include "core/object/inum/inum.h"
#include "core/object/nothing/nothing.h"
#include "core/object/ops/binop.h"
#include "core/object/ops/unop.h"
#include "core/object/string/string.h"
#include "err.h"

/*************/
/* Constants */
/*************/

char const* OBJ_TYPES_STR[OBJ_TYPES_COUNT] = {"integer number",
                                              "double-precision number",
                                              "string",
                                              "function",
                                              "C function"};

/***********/
/* Helpers */
/***********/

NoRet obj_free_f(struct Object* obj)
{
  /* Obvious check */
  DASSERT(obj, "");
  obj_decr_ref_count(CAST(obj, struct Object*));
}

NoRet obj_err_set_unsupported_unop(const enum ObjectUnOpType unop_type,
                                   const enum ObjectType op_type)
{
  err_setf(E_EXEC,
           "unsupported unary operation '%s' on object of type '%s'",
           obj_unop2str(unop_type),
           objt2str(op_type));
}

NoRet obj_err_set_unsupported_binop(const enum ObjectBinOpType binop_type,
                                    const enum ObjectType lop_type,
                                    const enum ObjectType rop_type)
{
  err_setf(E_EXEC,
           "unsupported binary operation '%s' on objects of type '%s' and '%s'",
           obj_binop2str(binop_type),
           objt2str(lop_type),
           objt2str(rop_type));
}

/***********/
/* Methods */
/***********/

NoRet obj_init(struct Object* obj, const enum ObjectType type)
{
  /* Chech that the object is not NULL */
  DASSERT(obj, "");
  /* Check that the given type is valid */
  DASSERT(OBJ_TYPE_IS_VALID(type), "");
  obj->type      = type;
  obj->ref_count = 1;
}

struct Object* obj_new(const enum ObjectType type)
{
  /* Check that the given type is valid */
  DASSERT(OBJ_TYPE_IS_VALID(type), "");
  /* Allocate memory for the new object */
  struct Object* obj;
  obj = xmalloc(sizeof(struct Object));
  /* Initialize that memory */
  obj_init(obj, type);
  /* Return the newly created object */
  return obj;
}

NoRet obj_delete(struct Object* obj)
{
  /* Obvious check */
  DASSERT(obj, "trying to remove NULL object");
  /* The way by which we deleting an object */
  /* varies depending on the type of a object */
  switch (obj->type) {
    case OBJT_INUM: obj_inum_delete(obj); break;
    case OBJT_DNUM: obj_dnum_delete(obj); break;
    case OBJT_STR: obj_str_delete(obj); break;
    case OBJT_FUNC: obj_func_delete(obj); break;
    default: IMPOSSIBLE();
  }
}

NoRet obj_decr_ref_count(struct Object* obj)
{
  /* Obvious check */
  DASSERT(obj, "");

  if (obj->ref_count == 0)
    obj_delete(obj);
  else if (!obj_is_reserved(obj)) {
    /* LOG_DEBUG( */
    /*     "decrementing the reference count of a object at \"%p\": %lu -> %lu",
     */
    /*     (void*)obj, obj->ref_count, obj->ref_count - 1); */
    --obj->ref_count;
  }
}

bool obj_is_reserved(const struct Object* obj)
{
  return (obj == &OBJ_STR_EMPTY) || (obj == &OBJ_NOTHING) ||
         (obj == &OBJ_TRUE) || (obj == &OBJ_FALSE);
}

NoRet obj_incr_ref_count(struct Object* obj)
{
  /* Obvious check */
  DASSERT(obj, "");

  /* Increment the actual ref count */
  if (!obj_is_reserved(obj)) {
    /* LOG_DEBUG( */
    /*     "incrementing the reference count of a object at \"%p\": %lu -> %lu",
     */
    /*     (void*)obj, obj->ref_count, obj->ref_count + 1); */
    ++obj->ref_count;
  }
}

NoRet obj_xvec_deinit(struct XVector* v)
{
  while (v->size > 0) {
    obj_decr_ref_count(xvec_get(v, v->size - 1));
    --v->size;
  }
}

NoRet obj_xvec_delete(struct XVector* v)
{
  obj_xvec_deinit(v);
  xfree(v);
}
