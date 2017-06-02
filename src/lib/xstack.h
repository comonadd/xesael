/* File: xstack.h */
/* Creation date: 17.12.2016 */
/* Author: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */

#ifndef XSTACK_H
#define XSTACK_H

#include "types.h"

/**
   @capacity - count of bytes that we allocated
   @size - count of items that we have
   @elems - array of elements
**/
struct XStack {
  uint64 capacity;
  uint64 size;
  void** elems;
};

NoRet xstack_init(struct XStack* s);
NoRet xstack_deinit(struct XStack* s);
void* xstack_pop(struct XStack* s);
NoRet xstack_push(struct XStack* s, void* item);
void* xstack_top(struct XStack* s);

#endif /* XSTACK_H */
