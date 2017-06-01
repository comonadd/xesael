/* File: str.c */
/* Creation Date: 2017-01-09 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* GSH string object */

#include "core/object/string/string.h"

#include <string.h>

#include "log.h"
#include "err.h"

#include "lib/types.h"
#include "lib/macros.h"
#include "lib/xmalloc.h"
#include "lib/util.h"
#include "lib/dict.h"

#include "core/object/object.h"
#include "core/object/inum/inum.h"
#include "core/object/ops/unop.h"

/*************/
/* Constants */
/*************/

const struct Object OBJ_STR_EMPTY = {
    .data = {.str = {0, NULL}},
    .ref_count = 1,
    .type = OBJT_STR
};

/***********/
/* Helpers */
/***********/

static const uint16 STR_OBJS_DICT_SIZE = 8192;
static struct Dictionary str_objs;

#define obj_str_find(val) dict_find(&str_objs, (val))
#define obj_str_insert(obj) dict_insert(&str_objs, (obj)->data.str.val, (obj))

NoRet str_objs_init(void)
{
    dict_init(
        &str_objs,
        STR_OBJS_DICT_SIZE,
        NULL, NULL,
        NULL, NULL);
}

NoRet str_objs_deinit(void)
{
    dict_deinit(&str_objs);
}

/***********/
/* Methods */
/***********/

struct Object* obj_str_new(
    const char* val)
{
    /* Obvious check */
    DASSERT(val, "");

    if (!val || (strlen(val) == 0))
        return CAST(&OBJ_STR_EMPTY, struct Object*);

    struct Object* obj = obj_str_find(val);
    if (obj) obj_incr_ref_count(obj);
    else {
        obj = obj_new(OBJT_STR);
        obj->data.str.len = strlen(val);
        obj->data.str.val = xmalloc((obj->data.str.len + 1) * sizeof(char));
        LOG_DEBUG(
            "creating string object at \"%p\" with value \"%s\"",
            (void*)obj,
            val);
        strcpy(obj->data.str.val, val);
        obj_str_insert(obj);
    }
    return obj;
}

NoRet obj_str_delete(struct Object* obj)
{
    /* Obvious check */
    DASSERT(obj, "");
    LOG_DEBUG(
        "removing string object at \"%p\" with value \"%s\"",
        (void*)obj,
        obj->data.str.val);
    /* Obvious check */
    OBJ_ASSERT_TYPE(obj, OBJT_STR);
    dict_remove(&str_objs, obj->data.str.val);
}

bool obj_str_is_true(
    const struct Object* obj)
{
    DASSERT(obj, "");
    return obj->data.str.len != 0;
}

struct Object* obj_str_unop(
    const enum ObjectUnOpType type,
    const struct Object* obj)
{
    /* Obvious checks */
    DASSERT(obj, "");
    OBJ_ASSERT_TYPE(obj, OBJT_STR);

    if (type == O_UNOP_LEN)
        return obj_inum_new(obj->data.str.len);

    obj_err_set_unsupported_unop(type, obj->type);
    return NULL;
}
