/* File: func.c */
/* Creation date: 2017-02-05 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* The function object */

#include "core/object/func/func.h"

#include "core/object/object.h"

struct Object* obj_func_new(void)
{
    return obj_new(OBJT_FUNC);
}

NoRet obj_func_delete(struct Object* obj)
{
    bytecode_delete(obj->data.func.bc);
    xfree(obj);
}
