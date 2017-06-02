/* File: cfunc.h */
/* Creation date: 2017-02-05 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* The CFunc object */

#ifndef CFUNC_H
#define CFUNC_H

#include "lib/types.h"
#include "lib/xvec.h"

#include "core/object/object.h"

/**
   @count - count of arguments
   @elems - the actual array of arguments
**/
struct CFuncArguments {
  struct XVector args;
};

typedef struct Object* (*CFuncImpl)(struct CFuncArguments*);

/**
   $ Description:
   #   Allocate a new C function arguments structure
**/
struct CFuncArguments* cfunc_args_new(void);

NoRet cfunc_args_append(struct CFuncArguments* args, struct Object const* arg);

struct Object const* cfunc_args_get(struct CFuncArguments* args,
                                    const uint64 i);

#endif /* CFUNC_H */
