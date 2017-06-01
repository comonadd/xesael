/* File: dict.h */
/* Creation Date: 2017-01-12*/
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* Generic dictionary implementation */

#ifndef DICT_H
#define DICT_H

#include "types.h"
#include "xmalloc.h"

#define dict_get_key_index(dict, key) ((dict)->hash_f(key) & (dict)->size)

/**
   $ Description:
   #   Dictionary entry
   @next - next entry in the slot
   @key - key of a entry
   @val - value of a entry
**/
struct DictionaryEntry {
    struct DictionaryEntry* next;
    void* key;
    void* val;
};

/**
   $ Description:
   #   When there is no elements,
   #   head == tail == NULL, and size == 0
   @size - count of entries in a slot
   @head - first entry in the slot
   @tail - last entry in the slot
**/
struct DictionarySlot {
    uint64 size;
    struct DictionaryEntry* head;
    struct DictionaryEntry* tail;
};

/**
   $ Description:
   #   The actual dictionary
   #   Collisions are handled by chaining
   @slots - array of slots in a dictionary
   @hash_f - function used to hash keys
   @key_cmp_f - function used to compare keys
   @key_free_f - function used to free key of a entry
   @val_free_f - function used to free value of a entry
**/
struct Dictionary {
    struct DictionarySlot* slots;
    uint32 size;
    uint32 (*hash_f)(const void* key);
    int (*key_cmp_f)(const void* key1, const void* key2);
    NoRet (*key_free_f)(void* key);
    NoRet (*val_free_f)(void* val);
};

/**************/
/* Operations */
/**************/

NoRet dict_init(
    struct Dictionary* dict,
    uint32 size,
    uint32 (*hash_f)(const void*),
    int (*key_cmp_f)(const void*, const void*),
    NoRet (*key_free_f)(void*),
    NoRet (*val_free_f)(void*));

NoRet dict_deinit(struct Dictionary* dict);

struct Dictionary* dict_new(
    uint32 size,
    uint32 (*hash_f)(const void*),
    int (*key_cmp_f)(const void*, const void*),
    NoRet (*key_free_f)(void*),
    NoRet (*val_free_f)(void*));

NoRet dict_delete(struct Dictionary* dict);

/**
   $ Description:
   #   This function adds a new
   #   item to the dictionary
   $ Return value:
   #   If item with that key already
   #   exists, returns false
   #   Returns true otherwise
   @existing_hash - if you already have
   #                an hash for a item
   #                you could just pass it
   #                (not to recalculate it)
**/
bool dict_insert_raw(
    struct Dictionary* dict,
    void* key,
    void* val,
    int64 existing_hash);

/**
   $ Description:
   #   This function is the same as dict_insert_raw()
   #   but it always passes -1 to it
**/
bool dict_insert(
    struct Dictionary* dict,
    void* key,
    void* val);

NoRet dict_replace(
    struct Dictionary* dict,
    void* key,
    void* val);

bool dict_remove(
    struct Dictionary* dict,
    void* key);

void* dict_find_raw(
    struct Dictionary const* dict,
    const void* key,
    int64 existing_hash);

void* dict_find(
    struct Dictionary const* dict,
    const void* key);

#  if defined(DICT_BENCH)
NoRet dict_bench(void);
#  endif /* defined(DICT_BENCH) */

#  if defined(DICT_TEST)
NoRet dict_test(void);
#  endif /* defined(DICT_TEST) */

#endif /* DICT_H */
