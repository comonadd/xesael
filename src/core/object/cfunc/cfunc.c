/* File: cfunc.c */
/* Creation date: 2017-02-05 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* The CFunc object */

#include "core/object/cfunc/cfunc.h"

#include "lib/types.h"
#include "lib/xmalloc.h"
#include "lib/xvec.h"

#include "core/object/object.h"

struct CFuncArguments* cfunc_args_new(void)
{
    struct CFuncArguments* args = xmalloc(sizeof(*args));
    if (!args) return NULL;
    xvec_init(&args->args, obj_free_f);
    return args;
}

NoRet cfunc_args_append(
    struct CFuncArguments* args,
    struct Object const* arg)
{
    xvec_append(&args->args, CAST(arg, void*));
}

struct Object const* cfunc_args_get(
    struct CFuncArguments* args,
    const uint64 i)
{
    return xvec_get(&args->args, i);
}
