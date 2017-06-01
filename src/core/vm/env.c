/* File: env.c */
/* Creation Date: 2017-01-29 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */

#include "core/vm/env.h"
#include "core/object/object.h"

#define ENV_VARS_DICT_SIZE 128

struct Environment* env_new(void)
{
    struct Environment* env = xmalloc(sizeof(*env));
    dict_init(
        &env->vars,
        ENV_VARS_DICT_SIZE,
        str_hash_f, NULL, NULL, obj_free_f);
    env->prev = NULL;
    return env;
}

NoRet env_delete(struct Environment* env)
{
    dict_deinit(&env->vars);
    if (env->prev) env_delete(env->prev);
}

NoRet env_set_var(
    struct Environment* env,
    char const* name,
    struct Object const* obj)
{
    dict_replace(&env->vars, CAST(name, char*), CAST(obj, struct Object*));
}

struct Object* env_get_var(
    struct Environment const* env,
    char const* name)
{
    struct Object* obj;
    const uint64 hash = dict_get_key_index(&env->vars, name);

repeat:
    if (!env) return NULL;
    obj = dict_find_raw(
        CAST(&env->vars, struct Dictionary*),
        name, hash);

    if (!obj) {
        env = env->prev;
        goto repeat;
    }

    return obj;
}
