/* File: func.h */
/* Creation date: 2017-02-05 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* The function object */

#ifndef OBJ_FUNC_H
#define OBJ_FUNC_H

#include "core/object/object.h"
#include "core/vm/bytecode/bytecode.h"

/**
   $ Description:
   #   This function creates a new function object
   @bc - the bytecode (body) of a function
**/
struct Object* obj_func_new(void);

NoRet obj_func_delete(struct Object* obj);

#endif /* OBJ_FUNC_H */
