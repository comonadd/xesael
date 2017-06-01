/* File: math.c */
/* Creation Date: 2017-01-11*/
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* GSh standard Math module */

#include <math.h>

#if 0
#include "shell/interpreter/modules/math.h"

#include "shell/interpreter/api.h"
#include "shell/interpreter/object/object.h"

static struct Object* gshstdlib_math_abs(
    struct ExecutorState* es,
    CallArguments* args)
{
    struct Object* arg = call_arg_get(args, 0);
    if (!(arg->type == OBJT_INUM || arg->type == OBJT_DNUM))
        return OBJ_NOTHING;
    if (arg->val < 0)
        return obj_inum_new(-arg->val);
    return arg;
}

NoRet module_math_load(struct Interpreter* is)
{
    /* Constants */
    struct Object* pi_obj = obj_dnum_new(3.14);
    struct Object* e_obj = obj_dnum_new(2.7);

    /* Functions */
    struct GShCFunction* abs_f = gsh_cfunc_new(
        "abs", 1, {gshstdlib_math_abs});

    /* The actual module */
    struct GShCModule* module = gshcmodule_new("math");
    gshcmodule_set_const(module, "PI", pi_obj);
    gshcmodule_set_const(module, "e", e_obj);
    gshcmodule_set_func(module, abs_f);

    /* Load the module */
    interpreter_load_module(is, module);
}

#endif /* 0 */
