/* File: binop.h */
/* Creation Date: 2017-01-23 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* Object binary operations */

#ifndef OBJ_BINOP_H
#define OBJ_BINOP_H

#include "core/object/object.h"

/* NOTE: O_BINOP_ADD should always be the first arithmetic operator */
/* NOTE: O_BINOP_RS should always be the last arithmetic operator */
/* NOTE: O_BINOP_EQ should always be the first comparison operator */
/* NOTE: O_BINOP_LOR should always be the last comparison operator */
enum ObjectBinOpType {
  /* Arithmetic */
  O_BINOP_ADD,
  O_BINOP_SUB,
  O_BINOP_MUL,
  O_BINOP_DIV,
  O_BINOP_MOD,
  O_BINOP_POW,
  O_BINOP_BAND,
  O_BINOP_BOR,
  O_BINOP_BXOR,
  O_BINOP_LS,
  O_BINOP_RS,

  /* Boolean */
  O_BINOP_LAND,
  O_BINOP_LOR,

  /* Comparison */
  O_BINOP_EQ,
  O_BINOP_NEQ,
  O_BINOP_LT,
  O_BINOP_GT,
  O_BINOP_LTE,
  O_BINOP_GTE
};

#define O_BINOP_ARITH_FIRST O_BINOP_ADD
#define O_BINOP_ARITH_LAST O_BINOP_RS

#define O_BINOP_BOOL_FIRST O_BINOP_LAND
#define O_BINOP_BOOL_LAST O_BINOP_LOR

#define O_BINOP_CMP_FIRST O_BINOP_EQ
#define O_BINOP_CMP_LAST O_BINOP_GTE

/***********/
/* Helpers */
/***********/

#define obj_binop_is_arith(__op) \
  ((__op >= O_BINOP_ARITH_FIRST) && (__op <= O_BINOP_ARITH_LAST))

#define obj_binop_is_bool(__op) \
  ((__op >= O_BINOP_BOOL_FIRST) && (__op <= O_BINOP_BOOL_LAST))

#define obj_binop_is_cmp(__op) \
  ((__op >= O_BINOP_CMP_FIRST) && (__op <= O_BINOP_CMP_LAST))

#define obj_binop2str(__type) TOKS_STR[TOK_BINOP_FIRST + (__type)]

/***********************************/
/* Binary objects "CMP" operations */
/***********************************/

struct Object* obj_binop_cmp_inum_dnum(const enum ObjectBinOpType type,
                                       const struct Object* lop,
                                       const struct Object* rop);

struct Object* obj_binop_cmp_inum(const enum ObjectBinOpType type,
                                  const struct Object* lop,
                                  const struct Object* rop);

struct Object* obj_binop_cmp_dnum(const enum ObjectBinOpType type,
                                  const struct Object* lop,
                                  const struct Object* rop);

struct Object* obj_binop_cmp_str(const enum ObjectBinOpType type,
                                 const struct Object* lop,
                                 const struct Object* rop);

/*************************************/
/* Binary objects "ARITH" operations */
/*************************************/

struct Object* obj_binop_arith_inum_dnum(const enum ObjectBinOpType binop,
                                         const struct Object* lop,
                                         const struct Object* rop);

struct Object* obj_binop_arith_inum_str(const enum ObjectBinOpType binop,
                                        const struct Object* lop,
                                        const struct Object* rop);

struct Object* obj_binop_arith_inum(const enum ObjectBinOpType binop,
                                    const struct Object* lop,
                                    const struct Object* rop);

struct Object* obj_binop_arith_dnum(const enum ObjectBinOpType binop,
                                    const struct Object* lop,
                                    const struct Object* rop);

struct Object* obj_binop_arith_str(const enum ObjectBinOpType binop,
                                   const struct Object* lop,
                                   const struct Object* rop);

/********/
/* Main */
/********/

struct Object* obj_binop_bool(const enum ObjectBinOpType binop,
                              const struct Object* lop,
                              const struct Object* rop);

struct Object* obj_binop_cmp(const enum ObjectBinOpType binop,
                             const struct Object* lop,
                             const struct Object* rop);

struct Object* obj_binop_arith(const enum ObjectBinOpType binop,
                               const struct Object* lop,
                               const struct Object* rop);

struct Object* obj_binop(const enum ObjectBinOpType binop,
                         const struct Object* lop,
                         const struct Object* rop);

#endif /* OBJ_BINOP_H */
