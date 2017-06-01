/* File: ops.h */
/* Creation Date: 2017-02-02 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* The helpers for operation on objects */

#ifndef OBJ_OPS_H
#define OBJ_OPS_H

#include "core/object/ops/unop.h"
#include "core/object/ops/binop.h"

/********************************/
/* Binary arithmetic operations */
/********************************/

#define obj_add(___lop, ___rop) obj_binop_arith(O_BINOP_ADD, (___lop), (___rop))
#define obj_sub(___lop, ___rop) obj_binop_arith(O_BINOP_SUB, (___lop), (___rop))
#define obj_mul(___lop, ___rop) obj_binop_arith(O_BINOP_MUL, (___lop), (___rop))
#define obj_div(___lop, ___rop) obj_binop_arith(O_BINOP_DIV, (___lop), (___rop))
#define obj_mod(___lop, ___rop) obj_binop_arith(O_BINOP_MOD, (___lop), (___rop))
#define obj_pow(___lop, ___rop) obj_binop_arith(O_BINOP_POW, (___lop), (___rop))
#define obj_band(___lop, ___rop) obj_binop_arith(O_BINOP_BAND, (___lop), (___rop))
#define obj_bor(___lop, ___rop) obj_binop_arith(O_BINOP_BOR, (___lop), (___rop))
#define obj_bxor(___lop, ___rop) obj_binop_arith(O_BINOP_BXOR, (___lop), (___rop))
#define obj_ls(___lop, ___rop) obj_binop_arith(O_BINOP_LS, (___lop), (___rop))
#define obj_rs(___lop, ___rop) obj_binop_arith(O_BINOP_RS, (___lop), (___rop))

/*****************************/
/* Binary boolean operations */
/*****************************/

#define obj_land(___lop, ___rop) obj_binop_cmp(O_BINOP_LAND, (___lop), (___rop))
#define obj_lor(___lop, ___rop) obj_binop_cmp(O_BINOP_LOR, (___lop), (___rop))

/********************************/
/* Binary comparison operations */
/********************************/

#define obj_eq(___lop, ___rop) obj_binop_cmp(O_BINOP_EQ, (___lop), (___rop))
#define obj_neq(___lop, ___rop) obj_binop_cmp(O_BINOP_NEQ, (___lop), (___rop))
#define obj_lt(___lop, ___rop) obj_binop_cmp(O_BINOP_LT, (___lop), (___rop))
#define obj_gt(___lop, ___rop) obj_binop_cmp(O_BINOP_GT, (___lop), (___rop))
#define obj_lte(___lop, ___rop) obj_binop_cmp(O_BINOP_LTE, (___lop), (___rop))
#define obj_gte(___lop, ___rop) obj_binop_cmp(O_BINOP_GTE, (___lop), (___rop))

/********************/
/* Unary operations */
/********************/

#define obj_neg(___op) obj_unop(O_UNOP_NEG,   (___op))
#define obj_lnot(___op) obj_unop(O_UNOP_LNOT, (___op))
#define obj_bnot(___op) obj_unop(O_UNOP_BNOT, (___op))
#define obj_len(___op) obj_unop(O_UNOP_LEN,   (___op))
#define obj_incr(___op) obj_unop(O_UNOP_INCR, (___op))
#define obj_decr(___op) obj_unop(O_UNOP_DECR, (___op))

#endif /* OBJ_OPS_H */
