/* File: immediate.h */
/* Creation Date: 2017-01-16*/
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* This thing is used in the bytecode as the representation */
/* of literals */

#ifndef VM_IMMEDIATE_H
#define VM_IMMEDIATE_H

#include "core/object/object.h"
#include "lib/types.h"

typedef uint64 ImmediateID;

/**
   @id - ID of a immediate (used only in a immediate lists)
   @obj - object that immediate is bound to
**/
struct Immediate {
  ImmediateID id;
  struct Object* obj;
};

/**
   $ Description:
   #   This function allocates a new immediate
   #   and initializes it. The ID is always set to 0
   $ Return value:
   #   Returns a pointer to a new immediate if succeeds.
   #   Returns NULL only if fails to allocate a memory
   #   (error code is set appropriately)
   @obj - object that immediate is bound to
**/
struct Immediate* immediate_new(struct Object* obj);

NoRet immediate_delete(struct Immediate* immediate);

NoRet immediate_free_f(void* ptr);

#endif /* VM_IMMEDIATE_H */
