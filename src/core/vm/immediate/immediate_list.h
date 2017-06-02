/* File: immediate_list.h */
/* Creation Date: 2017-01-16 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* The list of immediates */

#ifndef GSH_IMMEDIATE_LIST_H
#define GSH_IMMEDIATE_LIST_H

#include "core/vm/immediate/immediate.h"
#include "lib/bst.h"
#include "lib/types.h"

typedef struct BST ImmediateList;

NoRet immediate_list_init(ImmediateList* list);

NoRet immediate_list_deinit(ImmediateList* list);

struct Immediate* immediate_list_insert(ImmediateList* list,
                                        char const* name,
                                        struct Object* obj);

struct Immediate* immediate_list_lookup(ImmediateList const* list,
                                        char const* name);

struct Object* const* immediate_list_pack(ImmediateList const* list);

#endif /* GSH_IMMEDIATE_LIST_H */
