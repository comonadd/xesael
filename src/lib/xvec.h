/* File: xvec.h */
/* Creation date: 15.12.2016 */
/* Author: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* Generic vector implementation */

#ifndef XVEC_H
#define XVEC_H

#include "types.h"

/**
   @capacity - number of elements that we allocated memory for
   @size - count of actual elements
   @elems - elements of a vector
**/
struct XVector {
    uint64 capacity;
    uint64 size;
    void** elems;
    NoRet (*elem_free_f)(void* elem);
};

/***********/
/* Methods */
/***********/

/**
   $ Description:
   #   This function initializes the vector
   $ Input: not initialized vector
   $ Output: initialized vector that is ready for work
**/
bool xvec_init(
    struct XVector* v,
    NoRet (*elem_free_f)(void* elem));

/**
   $ Description:
   #   This function allocates and initializes the vector
   $ Input: nothing
   $ Output: new initialized vector that ready for work
**/
struct XVector* xvec_new(NoRet (*elem_free_f)(void* elem));

/**
   $ Description:
   #   This function de-initializes the vector and frees all memory allocated for it
   $ Input: initialized vector
   $ Output: freed memory for the vector
**/
/* NOTE: This is the GENERIC way of de-initializing vectors,   */
/* if you have some structs that should be de-initialized      */
/* some other way (than just by free()), you should write your */
/* own function for de-initializing vectors                    */
NoRet xvec_deinit(struct XVector* v);

/**
   $ Description:
   #   This function deletes and de-initializes the given vector
**/
NoRet xvec_delete(struct XVector* v);

/**
   This function sets value of thing at index in vector v to item
   Input: initialized vector, position to set to, item to set
   Output: item will be in vector at given position
   @index - position to set to in the vector
   @item - pointer to a new element
**/
bool xvec_set(
    struct XVector* v,
    const uint64 index,
    void* item);

/**
   $ Description:
   #   This function appends value of thing item to vector v
   $ Input: initialized vector and item to append
   $ Output: new item will be at the end of the vector
   @item - item to append
**/
NoRet xvec_append(
    struct XVector* v,
    void* item);

/**
   $ Description:
   #   This function returns value of item at index in vector v
   $ Input: vector to look value for and index at which value is stored
   $ Output: value at given position in the vector if exists
   #         NULL if not
   @index - position at which value is stored
**/
void* xvec_get(
    const struct XVector* v,
    const uint64 index);

/**
   $ Description:
   #   This function duplicates a given vector
   #   and returns a pointer to the duplicate
**/
struct XVector* xvec_dup(const struct XVector* v);

#endif /* XVEC_H */
