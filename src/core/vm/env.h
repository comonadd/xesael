/* File: env.h */
/* Creation Date: 2017-01-29 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */

#ifndef GSH_VM_ENV_H
#define GSH_VM_ENV_H

#include "lib/dict.h"
#include "lib/hash.h"
#include "core/object/object.h"

struct Environment {
    struct Dictionary vars;
    struct Environment* prev;
};

struct Environment* env_new(void);

NoRet env_delete(struct Environment* env);

NoRet env_set_var(
    struct Environment* env,
    char const* name,
    struct Object const* obj);

struct Object* env_get_var(
    struct Environment const* env,
    char const* name);

#endif /* GSH_VM_ENV_H */
