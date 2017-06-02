/* File: str.h */
/* Creation Date: 2017-01-09 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* String object */

#ifndef OBJ_STR_H
#define OBJ_STR_H

#include "lib/types.h"

#include "core/object/object.h"
#include "core/object/ops/unop.h"

/*************/
/* Constants */
/*************/

extern const struct Object OBJ_STR_EMPTY;

/***********/
/* Helpers */
/***********/

/**
   $ Description:
   #   This function initializes the dictionary
   #   in which we store all the objects
**/
NoRet str_objs_init(void);

NoRet str_objs_deinit(void);

/***********/
/* Methods */
/***********/

struct Object* obj_str_new(const char* val);

NoRet obj_str_delete(struct Object* obj);

bool obj_str_is_true(const struct Object* obj);

struct Object* obj_str_unop(const enum ObjectUnOpType type,
                            const struct Object* obj);

#endif /* OBJ_STR_H */
