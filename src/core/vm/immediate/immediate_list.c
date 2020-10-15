/* File: immediate_list.c */
/* Creation Date: 2017-01-16 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* The list of immediates */

#include "core/vm/immediate/immediate_list.h"

#include "core/vm/immediate/immediate.h"
#include "lib/bst.h"
#include "lib/macros.h"
#include "lib/str.h"
#include "lib/types.h"
#include "lib/util.h"
#include "lib/xmalloc.h"

NoRet immediate_list_init(ImmediateList* list)
{
  bst_init(CAST(list, struct BST*), str_cmp_f, free, immediate_free_f);
}

NoRet immediate_list_deinit(ImmediateList* list)
{
  bst_deinit(CAST(list, struct BST*));
}

struct Immediate* immediate_list_insert(ImmediateList* list,
                                        char* name,
                                        struct Object* obj)
{
  struct Immediate* I = bst_find(CAST(list, struct BST*), name);

  /* If already exists */
  if (!I) {
    I     = immediate_new(obj);
    I->id = CAST(list, struct BST*)->size;
    bst_insert(CAST(list, struct BST*), name, I);
  }
  return I;
}

struct Immediate* immediate_list_lookup(ImmediateList const* list,
                                        char const* name)
{
  return bst_find(CAST(list, struct BST*), name);
}

/*******************/
/* "Pack" function */
/*******************/

struct Object** __tmp = NULL;

static NoRet immediate_list_pack_node_f(struct BSTNode* node)
{
  struct Immediate* immediate = node->val;
  __tmp[immediate->id]        = immediate->obj;
}

struct Object* const* immediate_list_pack(ImmediateList const* list)
{
  DASSERT(list, "");
  if (list->size > 0) {
    __tmp = xmalloc(list->size * sizeof(*__tmp));
    bst_apply(CAST(list, struct BST*), immediate_list_pack_node_f);
    return __tmp;
  }
  return NULL;
}
