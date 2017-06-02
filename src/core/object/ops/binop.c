/* File: binop.c */
/* Creation Date: 2017-01-23 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* Object binary operations */

#include "core/object/ops/binop.h"

#include <math.h>

#include "err.h"

#include "lib/str.h"
#include "lib/util.h"

#include "core/object/bool/bool.h"
#include "core/object/dnum/dnum.h"
#include "core/object/inum/inum.h"
#include "core/object/nothing/nothing.h"
#include "core/object/object.h"
#include "core/object/string/string.h"

/***********************************/
/* Binary objects "CMP" operations */
/***********************************/

struct Object* obj_binop_cmp_inum_dnum(const enum ObjectBinOpType type,
                                       const struct Object* lop,
                                       const struct Object* rop)
{
  /* Obvious checks */
  DASSERT(obj_binop_is_cmp(type), "");
  DASSERT(lop, "");
  DASSERT(rop, "");
  OBJ_ASSERT_TYPE(lop, OBJT_INUM);
  OBJ_ASSERT_TYPE(rop, OBJT_DNUM);

  const double lop_val = lop->data.inum.val;
  const double rop_val = rop->data.dnum.val;
  struct Object* tobj  = CAST(&OBJ_TRUE, struct Object*);
  struct Object* fobj  = CAST(&OBJ_FALSE, struct Object*);

  switch (type) {
    case O_BINOP_EQ: return (dbl_eq(lop_val, rop_val)) ? tobj : fobj;
    case O_BINOP_NEQ: return (dbl_neq(lop_val, rop_val)) ? tobj : fobj;
    case O_BINOP_LT: return (dbl_lt(lop_val, rop_val)) ? tobj : fobj;
    case O_BINOP_GT: return (dbl_gt(lop_val, rop_val)) ? tobj : fobj;
    case O_BINOP_LTE: return (dbl_lte(lop_val, rop_val)) ? tobj : fobj;
    case O_BINOP_GTE: return (dbl_gte(lop_val, rop_val)) ? tobj : fobj;
    case O_BINOP_LAND:
      return (obj_dnum_is_true(lop) && obj_dnum_is_true(rop)) ? tobj : fobj;
    case O_BINOP_LOR:
      return (obj_dnum_is_true(lop) || obj_dnum_is_true(rop)) ? tobj : fobj;
    default: IMPOSSIBLE();
  }

  return NULL;
}

struct Object* obj_binop_cmp_inum(const enum ObjectBinOpType type,
                                  const struct Object* lop,
                                  const struct Object* rop)
{
  /* Obvious checks */
  DASSERT(obj_binop_is_cmp(type), "");
  DASSERT(lop, "");
  DASSERT(rop, "");

  switch (rop->type) {
    case OBJT_INUM: {
      const int64 lop_val = lop->data.inum.val;
      const int64 rop_val = rop->data.inum.val;
      struct Object* tobj = CAST(&OBJ_TRUE, struct Object*);
      struct Object* fobj = CAST(&OBJ_FALSE, struct Object*);

      switch (type) {
        case O_BINOP_EQ: return (lop_val == rop_val) ? tobj : fobj;
        case O_BINOP_NEQ: return (lop_val != rop_val) ? tobj : fobj;
        case O_BINOP_LT: return (lop_val < rop_val) ? tobj : fobj;
        case O_BINOP_GT: return (lop_val > rop_val) ? tobj : fobj;
        case O_BINOP_LTE: return (lop_val <= rop_val) ? tobj : fobj;
        case O_BINOP_GTE: return (lop_val >= rop_val) ? tobj : fobj;
        case O_BINOP_LAND: return (lop_val && rop_val) ? tobj : fobj;
        case O_BINOP_LOR: return (lop_val || rop_val) ? tobj : fobj;
        default: IMPOSSIBLE(); return NULL;
      }
    }
    case OBJT_DNUM: return obj_binop_cmp_inum_dnum(type, lop, rop);
    default: obj_err_set_unsupported_binop(type, lop->type, rop->type);
  }

  return NULL;
}

struct Object* obj_binop_cmp_dnum(const enum ObjectBinOpType type,
                                  const struct Object* lop,
                                  const struct Object* rop)
{
  /* Obvious checks */
  DASSERT(obj_binop_is_cmp(type), "");
  DASSERT(lop, "");
  DASSERT(rop, "");
  OBJ_ASSERT_TYPE(lop, OBJT_DNUM);

  switch (rop->type) {
    case OBJT_DNUM: {
      const double lop_val = lop->data.dnum.val;
      const double rop_val = rop->data.dnum.val;
      struct Object* tobj  = CAST(&OBJ_TRUE, struct Object*);
      struct Object* fobj  = CAST(&OBJ_FALSE, struct Object*);

      switch (type) {
        case O_BINOP_EQ: return (dbl_eq(lop_val, rop_val)) ? tobj : fobj;
        case O_BINOP_NEQ: return (dbl_neq(lop_val, rop_val)) ? tobj : fobj;
        case O_BINOP_LT: return (dbl_lt(lop_val, rop_val)) ? tobj : fobj;
        case O_BINOP_GT: return (dbl_gt(lop_val, rop_val)) ? tobj : fobj;
        case O_BINOP_LTE: return (dbl_lte(lop_val, rop_val)) ? tobj : fobj;
        case O_BINOP_GTE: return (dbl_gte(lop_val, rop_val)) ? tobj : fobj;
        case O_BINOP_LAND:
          return (obj_dnum_is_true(lop) && obj_dnum_is_true(rop)) ? tobj : fobj;
        case O_BINOP_LOR:
          return (obj_dnum_is_true(lop) || obj_dnum_is_true(rop)) ? tobj : fobj;
        default: IMPOSSIBLE(); return NULL;
      }
    }
    case OBJT_INUM: return obj_binop_cmp_inum_dnum(type, lop, rop);
    default: obj_err_set_unsupported_binop(type, lop->type, rop->type);
  }

  return NULL;
}

struct Object* obj_binop_cmp_str(const enum ObjectBinOpType type,
                                 const struct Object* lop,
                                 const struct Object* rop)
{
  /* Obvious checks */
  DASSERT(lop, "");
  DASSERT(rop, "");
  DASSERT(obj_binop_is_cmp(type), "");
  OBJ_ASSERT_TYPE(lop, OBJT_STR);

  switch (rop->type) {
    case OBJT_STR: {
      const char* lop_val = lop->data.str.val;
      const char* rop_val = rop->data.str.val;
      struct Object* tobj = CAST(&OBJ_TRUE, struct Object*);
      struct Object* fobj = CAST(&OBJ_FALSE, struct Object*);

      int cmp_res = strcmp(lop_val, rop_val);
      switch (type) {
        case O_BINOP_EQ: return (cmp_res == 0) ? tobj : fobj;
        case O_BINOP_NEQ: return (cmp_res != 0) ? tobj : fobj;
        case O_BINOP_LT: return (cmp_res < 0) ? tobj : fobj;
        case O_BINOP_GT: return (cmp_res > 0) ? tobj : fobj;
        case O_BINOP_LTE: return (cmp_res <= 0) ? tobj : fobj;
        case O_BINOP_GTE: return (cmp_res >= 0) ? tobj : fobj;
        default: IMPOSSIBLE(); return NULL;
      }
    }
    default:
      obj_err_set_unsupported_binop(type, lop->type, rop->type);
      return NULL;
  }
}

/*************************************/
/* Binary objects "ARITH" operations */
/*************************************/

struct Object* obj_binop_arith_inum_dnum(const enum ObjectBinOpType binop,
                                         const struct Object* lop,
                                         const struct Object* rop)
{
  /* Obvious checks */
  DASSERT(lop, "");
  DASSERT(rop, "");
  OBJ_ASSERT_TYPE(lop, OBJT_INUM);
  OBJ_ASSERT_TYPE(rop, OBJT_DNUM);
  DASSERT(obj_binop_is_arith(binop), "");

  const double lop_val = CAST(lop->data.inum.val, double);
  const double rop_val = rop->data.dnum.val;

  switch (binop) {
    case O_BINOP_ADD: return obj_dnum_new(lop_val + rop_val);
    case O_BINOP_SUB: return obj_dnum_new(lop_val - rop_val);
    case O_BINOP_MUL: return obj_dnum_new(lop_val * rop_val);
    case O_BINOP_DIV: return obj_dnum_new(lop_val / rop_val);
    case O_BINOP_POW: return obj_dnum_new(pow(lop_val, rop_val));
    default:
      obj_err_set_unsupported_binop(binop, lop->type, rop->type);
      return NULL;
  }
}

struct Object* obj_binop_arith_inum_str(const enum ObjectBinOpType binop,
                                        const struct Object* lop,
                                        const struct Object* rop)
{
  /* Obvious checks */
  DASSERT(lop, "");
  DASSERT(rop, "");
  OBJ_ASSERT_TYPE(lop, OBJT_INUM);
  OBJ_ASSERT_TYPE(rop, OBJT_STR);
  DASSERT(obj_binop_is_arith(binop), "");

  const int64 lop_val  = lop->data.inum.val;
  const char* rop_val  = rop->data.str.val;
  const uint64 rop_len = rop->data.str.len;
  switch (binop) {
    case O_BINOP_MUL: return obj_str_new(str_mul(rop_val, rop_len, lop_val));
    default:
      obj_err_set_unsupported_binop(binop, lop->type, rop->type);
      return NULL;
  }
}

struct Object* obj_binop_arith_inum(const enum ObjectBinOpType binop,
                                    const struct Object* lop,
                                    const struct Object* rop)
{
  /* Obvious checks */
  DASSERT(lop, "");
  DASSERT(rop, "");
  OBJ_ASSERT_TYPE(lop, OBJT_INUM);
  DASSERT(obj_binop_is_arith(binop), "");

  switch (rop->type) {
    case OBJT_INUM: {
      const int64 lop_val = lop->data.inum.val;
      const int64 rop_val = rop->data.inum.val;
      switch (binop) {
        case O_BINOP_ADD: return obj_inum_new(lop_val + rop_val);
        case O_BINOP_SUB: return obj_inum_new(lop_val - rop_val);
        case O_BINOP_MUL: return obj_inum_new(lop_val * rop_val);
        case O_BINOP_DIV: return obj_inum_new(lop_val / rop_val);
        case O_BINOP_MOD: return obj_inum_new(lop_val % rop_val);
        case O_BINOP_POW: return obj_inum_new(pow(lop_val, rop_val));
        case O_BINOP_BAND: return obj_inum_new(lop_val & rop_val);
        case O_BINOP_BOR: return obj_inum_new(lop_val | rop_val);
        case O_BINOP_BXOR: return obj_inum_new(lop_val ^ rop_val);
        case O_BINOP_LS: return obj_inum_new(lop_val << rop_val);
        case O_BINOP_RS: return obj_inum_new(lop_val >> rop_val);
        default: IMPOSSIBLE(); return NULL;
      }
    }
    case OBJT_DNUM: return obj_binop_arith_inum_dnum(binop, lop, rop);
    case OBJT_STR: return obj_binop_arith_inum_str(binop, lop, rop);
    default:
      obj_err_set_unsupported_binop(binop, lop->type, rop->type);
      return NULL;
  }
}

struct Object* obj_binop_arith_dnum(const enum ObjectBinOpType binop,
                                    const struct Object* lop,
                                    const struct Object* rop)
{
  /* Obvious check */
  DASSERT(lop, "");
  DASSERT(rop, "");
  OBJ_ASSERT_TYPE(lop, OBJT_DNUM);
  switch (rop->type) {
    case OBJT_INUM: return obj_binop_arith_inum_dnum(binop, rop, lop);
    case OBJT_DNUM: {
      const double lop_val = lop->data.dnum.val;
      const double rop_val = rop->data.dnum.val;
      switch (binop) {
        case O_BINOP_ADD: return obj_dnum_new(lop_val + rop_val);
        case O_BINOP_SUB: return obj_dnum_new(lop_val - rop_val);
        case O_BINOP_MUL: return obj_dnum_new(lop_val * rop_val);
        case O_BINOP_DIV: return obj_dnum_new(lop_val / rop_val);
        case O_BINOP_MOD: return obj_dnum_new(fmod(lop_val, rop_val));
        case O_BINOP_POW: return obj_dnum_new(pow(lop_val, rop_val));
        default: IMPOSSIBLE(); return NULL;
      }
    }
    default:
      obj_err_set_unsupported_binop(binop, lop->type, rop->type);
      return NULL;
  }
}

struct Object* obj_binop_arith_str(const enum ObjectBinOpType binop,
                                   const struct Object* lop,
                                   const struct Object* rop)
{
  /* Obvious check */
  OBJ_ASSERT_TYPE(lop, OBJT_STR);
  switch (rop->type) {
    case OBJT_INUM: return obj_binop_arith_inum_str(binop, rop, lop);
    case OBJT_STR: {
      const char* lop_val = lop->data.str.val;
      const char* rop_val = rop->data.str.val;
      switch (binop) {
        case O_BINOP_ADD: return obj_str_new(str_concat(lop_val, rop_val));
        default: goto out_unsupported_binop;
      }
    }
    default: goto out_unsupported_binop;
  }
out_unsupported_binop:
  obj_err_set_unsupported_binop(binop, lop->type, rop->type);
  return NULL;
}

/********/
/* Main */
/********/

struct Object* obj_binop_bool(const enum ObjectBinOpType binop,
                              const struct Object* lop,
                              const struct Object* rop)
{
  struct Object* tobj = CAST(&OBJ_TRUE, struct Object*);
  struct Object* fobj = CAST(&OBJ_FALSE, struct Object*);
  switch (binop) {
    case O_BINOP_LAND:
      return (obj_is_true(lop) && obj_is_true(rop)) ? tobj : fobj;
    case O_BINOP_LOR:
      return (obj_is_true(lop) || obj_is_true(rop)) ? tobj : fobj;
    default: IMPOSSIBLE(); return NULL;
  }
}

struct Object* obj_binop_cmp(const enum ObjectBinOpType binop,
                             const struct Object* lop,
                             const struct Object* rop)
{
  /* Obvious check */
  DASSERT(obj_binop_is_cmp(binop), "");
  DASSERT(lop, "");
  DASSERT(rop, "");
  switch (lop->type) {
    case OBJT_INUM: return obj_binop_cmp_inum(binop, lop, rop);
    case OBJT_DNUM: return obj_binop_cmp_dnum(binop, lop, rop);
    case OBJT_STR: return obj_binop_cmp_str(binop, lop, rop);
    default: IMPOSSIBLE(); return NULL;
  }
}

struct Object* obj_binop_arith(const enum ObjectBinOpType binop,
                               const struct Object* lop,
                               const struct Object* rop)
{
  /* Ovbious check */
  DASSERT(obj_binop_is_arith(binop), "");
  DASSERT(lop, "");
  DASSERT(rop, "");
  switch (lop->type) {
    case OBJT_INUM: return obj_binop_arith_inum(binop, lop, rop);
    case OBJT_DNUM: return obj_binop_arith_dnum(binop, lop, rop);
    case OBJT_STR: return obj_binop_arith_str(binop, lop, rop);
    default: IMPOSSIBLE(); return NULL;
  }
}

struct Object* obj_binop(const enum ObjectBinOpType binop,
                         const struct Object* lop,
                         const struct Object* rop)
{
  /* Obvious checks */
  DASSERT(lop, "");
  DASSERT(rop, "");

  /* If given binary operation is a boolean operation */
  if (obj_binop_is_bool(binop)) return obj_binop_bool(binop, lop, rop);

  /* If given binary operaetion is a arithmetic operation */
  if (obj_binop_is_arith(binop)) return obj_binop_arith(binop, lop, rop);

  /* If given binary operation is a comparison operation */
  return obj_binop_cmp(binop, lop, rop);
}
