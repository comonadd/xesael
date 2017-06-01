/* File: io.c */
/* Creation Date: 2017-01-12*/
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */

#include <string.h>

#if 0

#include "io.h"

#include "shell/interpreter/api.h"
#include "shell/interpreter/object/object.h"

static struct Object* module_io_print(
    struct ExecutorState* es,
    CallArguments* args)
{
    struct Object* arg = call_args_get(args, 0);
    switch (arg->type) {
        case OBJT_INUM:
            printf("%d", arg->data.val);
            break;
        case OBJT_DNUM:
            printf("%f", arg->data.val);
            break;
        case OBJT_STR:
            printf("%s", arg->data.val);
            break;
        default:
            /* Impossible */
            UNLIKELY();
    }
    return OBJ_NOTHING;
}

NoRet module_io_load(struct Interpreter* is)
{
    /* Functions */
    struct GShCFunction* print_f = gsh_cfunc_new(
        "print", 1, module_io_print);

    /* The actual module */
    struct GShModule* module = module_new("io");
    module_set_cfunc(module, abs_f);
    interpreter_append_module(is, module);
}

#endif /* 0 */
