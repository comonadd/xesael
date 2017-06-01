/* File: unop.h */
/* Creation Date: 2017-01-23 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* Object unary operations */

#ifndef OBJ_UNOP_H
#define OBJ_UNOP_H

#include "core/object/object.h"

enum ObjectUnOpType {
    O_UNOP_NEG,
    O_UNOP_LNOT,
    O_UNOP_BNOT,
    O_UNOP_LEN,
    O_UNOP_INCR,
    O_UNOP_DECR,
    O_UNOP_COUNT
};

/***********/
/* Helpers */
/***********/

#define OBJ_ASSERT_UNOPT_VALID(__type) ((__type) < O_UNOP_COUNT)
#define obj_unop2str(__type) TOKS_STR[TOK_UNOP_FIRST + (__type)]

/********/
/* Main */
/********/

struct Object* obj_unop(
    const enum ObjectUnOpType type,
    struct Object const* obj);

#endif /* OBJ_UNOP_H */
