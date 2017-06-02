/* File: object.h */
/* Creation Date: 2017-01-06*/
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* The object */

#ifndef OBJ_H
#define OBJ_H

#include "err.h"

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/xvec.h"

#include "core/compiler/token/token.h"

#include "core/object/ops/binop.h"
#include "core/object/ops/unop.h"

/* Forward declaration */
struct CFuncArguments;

/**
   @OBJT_INUM - integer number literal object
   @OBJT_DNUM - double-precision number literal object
   @OBJT_STR - string object
   @OBJT_FUNC - function object
   @OBJT_CFUNC - C function object
**/
enum ObjectType {
  OBJT_INUM,
  OBJT_DNUM,
  OBJT_STR,
  OBJT_FUNC,
  OBJT_CFUNC,
  OBJ_TYPES_COUNT
};

union ObjectData {
  /* Integer number */
  struct {
    int64 val;
  } inum;

  /* Double-precision number */
  struct {
    double val;
  } dnum;

  /* String */
  struct {
    uint64 len;
    char* val;
  } str;

  /* Function */
  struct {
    struct ByteCode* bc;
  } func;

  /* C Function */
  struct {
    struct Object* (*f)(struct CFuncArguments* args);
  } cfunc;
};

/**
   Structure that represents an object
   @data - data that object contain
   @ref_count - count of references to this object
   @type - type of a object
**/
struct Object {
  union ObjectData data;
  uint64 ref_count;
  enum ObjectType type;
};

/*************/
/* Constants */
/*************/

extern char const* OBJ_TYPES_STR[OBJ_TYPES_COUNT];

/************************/
/* Forward declarations */
/************************/

enum ObjectUnOpType;
enum ObjectBinOpType;

/***********/
/* Helpers */
/***********/

#define OBJ_ASSERT_TYPE(__obj, __t) DASSERT(((__obj)->type == (__t)), "")
#define OBJ_TYPE_IS_VALID(__type) ((__type) < OBJ_TYPES_COUNT)

#define objt2str(___t) OBJ_TYPES_STR[CAST(___t, uint64)]
#define obj2str(___obj) objt2str(CAST(___obj, struct Object*)->type)
#define obj_print(___obj)                                                \
  do {                                                                   \
    char const* type_str = objt2str(CAST(___obj, struct Object*)->type); \
    printf("<%p> {%s}", CAST(___obj, void*), type_str);                  \
  } while (0)

NoRet obj_err_set_unsupported_unop(const enum ObjectUnOpType unop_type,
                                   const enum ObjectType op_type);

NoRet obj_err_set_unsupported_binop(const enum ObjectBinOpType binop_type,
                                    const enum ObjectType lop_type,
                                    const enum ObjectType rop_type);

NoRet obj_free_f(struct Object* obj);

/***********/
/* Methods */
/***********/

NoRet obj_init(struct Object* obj, const enum ObjectType type);

struct Object* obj_new(const enum ObjectType type);

NoRet obj_delete(struct Object* obj);

bool obj_is_reserved(const struct Object* obj);

/* NOTE: If object count of references is <= 1, it will be removed */
NoRet obj_decr_ref_count(struct Object* obj);

NoRet obj_incr_ref_count(struct Object* obj);

NoRet obj_xvec_deinit(struct XVector* v);

NoRet obj_xvec_delete(struct XVector* v);

#endif /* OBJ_H */
