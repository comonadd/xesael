/* File: dict.c */
/* Creation Date: 2017-01-12*/
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* Generic dictionary implementation */

#include "dict.h"

#include "types.h"
#include "macros.h"
#include "xmalloc.h"
#include "util.h"
#include "str.h"
#include "hash.h"

#define dict_slot_incr_size(slot) (++slot->size)
#define dict_slot_decr_size(slot) (--slot->size)
#define dict_slot_is_empty(slot) (slot->size == 0)
#define dict_slot_keys_eq(dict, key1, key2)			\
    ((key1 == key2) || (dict->key_cmp_f(key1, key2) == 0))

/*************************/
/* Dictionary slot entry */
/*************************/

static struct DictionaryEntry* dict_slot_entry_new(
    void* key,
    void* val)
{
    struct DictionaryEntry* entry;
    entry = xmalloc(sizeof(struct DictionaryEntry));
    entry->key = key;
    entry->val = val;
    entry->next = NULL;
    return entry;
}

static NoRet dict_slot_entry_delete(
    struct Dictionary* dict,
    struct DictionaryEntry* entry)
{
    DASSERT(dict, "");
    DASSERT(entry, "");

    /* Free the key of an entry */
    if (entry->key) dict->key_free_f(entry->key);

    /* Free the value of an entry */
    if (entry->val) dict->val_free_f(entry->val);

    /* Free the actual entry */
    xfree(entry);
}

/*******************/
/* Dictionary slot */
/*******************/

static NoRet dict_slot_init(
    struct DictionarySlot* slot)
{
    slot->size = 0;
    slot->head = NULL;
    slot->tail = NULL;
}

static NoRet dict_slot_deinit(
    struct Dictionary* dict,
    struct DictionarySlot* slot)
{
    /* Start remove from the head */
    struct DictionaryEntry* entry;
    entry = slot->head;
    /* Until we reach the end of a slot */
    while (entry) {
        struct DictionaryEntry* next;
        next = entry->next;
        /* Delete the entry */
        dict_slot_entry_delete(dict, entry);
        /* Go to the next entry */
        entry = next;
    }
}

/* NOTE: This function does not handle the existance of entry with that key already */
/*       so, you should probably do it YOURSELF */
static NoRet dict_slot_append(
    struct DictionarySlot* slot,
    struct DictionaryEntry* entry)
{
    /* If slot is empty */
    if (dict_slot_is_empty(slot)) {
        /* If there are no entries at all */
        /* Just fill the head */
        slot->head = entry;
        dict_slot_incr_size(slot);
        return;
    }

    /* Is slot has only a head */
    if (slot->size == 1) {
        /* If there are head, but there */
        /* is no tail */
        /* Just fill the tail */
        slot->tail = entry;
        slot->head->next = slot->tail;
        dict_slot_incr_size(slot);
        return;
    }

    /* If there are an head and a tail */
    /* Change the tail next pointer to a */
    /* new entry */
    slot->tail->next = entry;
    /* Say that the tail is now the new entry */
    slot->tail = entry;
    /* Increment the size */
    dict_slot_incr_size(slot);
}

static bool dict_slot_remove(
    struct Dictionary* dict,
    struct DictionarySlot* slot,
    void* key)
{
    /* Obvious checks */
    DASSERT(dict, "");
    DASSERT(slot, "");
    DASSERT(slot->head, "");

    /* Check if we want to remove a head */
    if (dict_slot_keys_eq(dict, slot->head->key, key)) {
        /* Save the old head */
        struct DictionaryEntry* old_head;
        old_head = slot->head;
        /* Say that the new head is the old head next */
        slot->head = old_head->next;
        /* Deallocate the old head */
        dict_slot_entry_delete(dict, old_head);
        goto done;
    }

    /* Start search for the element that we wanna to remove */
    /* from the head */
    struct DictionaryEntry* entry;
    entry = slot->head;
    while (entry->next) {
        if (dict_slot_keys_eq(dict, entry->next->key, key)) {
            /* Save the ptr to the entry that we wanna to remove */
            struct DictionaryEntry* entry_to_remove;
            entry_to_remove = entry->next;
            if (entry_to_remove == slot->tail) {
                /* If we wanted to remove a tail */
                /* Change the slot tail to the */
                /* entry that is the before the tail */
                slot->tail = entry;
                /* Tail next is always the NULL */
                slot->tail->next = NULL;
            } else {
                /* Change the pointer of previous entry */
                entry->next = entry->next->next;
            }
            /* Deallocate entry that we wanna to remove */
            dict_slot_entry_delete(dict, entry_to_remove);
            goto done;
        }
        /* Go to the next entry */
        entry = entry->next;
    }

    /* We not found element that we wanted to remove */
    return false;

done:
    /* Deletion has been successfully done */
    dict_slot_decr_size(slot);
    return true;
}

static struct DictionaryEntry* dict_slot_find(
    struct Dictionary const* dict,
    struct DictionarySlot* slot,
    const void* key)
{
    /* Start search from the head */
    struct DictionaryEntry* entry = slot->head;
    /* While there are entry */
    while (entry) {
        /* If keys are equal */
        if ((entry->key == key) || (dict->key_cmp_f(entry->key, key) == 0)) {
            /* Found needed entry */
            return entry;
        }
        /* Go to the next entry */
        entry = entry->next;
    }
    /* Not found anything */
    return NULL;
}

/**************/
/* Dictionary */
/**************/

NoRet dict_init(
    struct Dictionary* dict,
    uint32 size,
    uint32 (*hash_f)(const void*),
    int (*key_cmp_f)(const void*, const void*),
    NoRet (*key_free_f)(void*),
    NoRet (*val_free_f)(void*))
{
    dict->size = size;
    dict->slots = xmalloc(size * sizeof(struct DictionarySlot));

    for (uint32 i = 0; i < size; ++i)
        dict_slot_init(&dict->slots[i]);

    /* If hash function to use is specified */
    if (hash_f) dict->hash_f = hash_f;
    else {
        /* If not, use standard */
        dict->hash_f = str_hash_f;
    }

    /* If key compare function is specified */
    if (key_cmp_f) dict->key_cmp_f = key_cmp_f;
    else {
        /* If not, use standard */
        dict->key_cmp_f = str_cmp_f;
    }

    /* If function for key deallocation is specified */
    if (key_free_f) dict->key_free_f = key_free_f;
    else {
        /* If not, use standard */
        dict->key_free_f = xfree;
    }

    /* If function for value deallocation is specified */
    if (val_free_f) dict->val_free_f = val_free_f;
    else {
        /* If not, use standard */
        dict->val_free_f = xfree;
    }
}

NoRet dict_deinit(struct Dictionary* dict)
{
    for (uint32 i = 0; i < dict->size; ++i) {
        if (dict->slots[i].size > 0) {
            dict_slot_deinit(dict, &dict->slots[i]);
        }
    }
    xfree(dict->slots);
}

struct Dictionary* dict_new(
    uint32 size,
    uint32 (*hash_f)(const void*),
    int (*key_cmp_f)(const void*, const void*),
    NoRet (*key_free_f)(void*),
    NoRet (*val_free_f)(void*))
{
    struct Dictionary* dict;
    dict = xmalloc(sizeof(struct Dictionary));
    dict_init(
        dict,
        size,
        hash_f,
        key_cmp_f,
        key_free_f,
        val_free_f);
    return dict;
}

NoRet dict_delete(struct Dictionary* dict)
{
    DASSERT(dict, "");
    dict_deinit(dict);
    xfree(dict);
}

bool dict_insert_raw(
    struct Dictionary* dict,
    void* key,
    void* val,
    int64 existing_hash)
{
    /* Retreive the index of a slot */
    uint32 i;
    i = (existing_hash == -1) ?
        dict_get_key_index(dict, key) :
        CAST(existing_hash, uint32);
    /* Retreive the slot */
    struct DictionarySlot* slot = &dict->slots[i];
    /* If slot already contains entry with a given key */
    if (dict_slot_find(dict, slot, key)) return false;
    /* Create a new entry */
    struct DictionaryEntry* entry;
    entry = dict_slot_entry_new(key, val);
    /* Append a new entry to the slot */
    dict_slot_append(slot, entry);
    return true;
}

bool dict_insert(
    struct Dictionary* dict,
    void* key,
    void* val)
{
    return dict_insert_raw(dict, key, val, -1);
}

NoRet dict_replace(
    struct Dictionary* dict,
    void* key,
    void* val)
{
    /* Retreive the index of a slot */
    uint32 i = dict_get_key_index(dict, key);

    /* Retreive the slot */
    struct DictionarySlot* slot = &dict->slots[i];
    struct DictionaryEntry* entry;

    /* Check if the entry with a given key already */
    /* exists in a slot */
    entry = dict_slot_find(dict, slot, key);
    if (entry) {
        /* Free the previous value */
        dict->val_free_f(entry->val);
        /* Set the new value */
        entry->val = val;
        return;
    }

    /* Append a new entry to the slot */
    /* Create a new entry */
    entry = dict_slot_entry_new(key, val);
    dict_slot_append(slot, entry);
}

bool dict_remove(
    struct Dictionary* dict,
    void* key)
{
    /* Retreive the index of a slot */
    uint32 i = dict_get_key_index(dict, key);
    /* Retreive the slot */
    struct DictionarySlot* slot = &dict->slots[i];
    if (!slot->head) return false;
    /* Remove an entry in a slot */
    return dict_slot_remove(dict, slot, key);
}

void* dict_find_raw(
    struct Dictionary const* dict,
    const void* key,
    int64 existing_hash)
{
    /* Retreive the index of a slot to find in */
    uint32 i = (existing_hash == -1) ?
        dict_get_key_index(dict, key) :
        existing_hash;
    /* Retreive the slot to find in */
    struct DictionarySlot* slot = &dict->slots[i];
    /* Find in a slot */
    struct DictionaryEntry* entry;
    entry = dict_slot_find(dict, slot, key);
    return entry ? entry->val : NULL;
}

void* dict_find(
    struct Dictionary const* dict,
    const void* key)
{
    return dict_find_raw(dict, key, -1);
}

/**************/
/* Benchmarks */
/**************/

#if defined(DICT_BENCH)
#include <sys/time.h>
#include <assert.h>

#define KEY_LEN 8
#define ITER_COUNT CAST((CAST(2, uint64) << CAST(24, uint64)), uint64)
#define DICT_BENCH_DICT_SIZE 8192

#define DICT_BENCH_INIT(name)						\
    struct Dictionary dict;						\
    struct timeval tval_before, tval_after, tval_result;		\
    dict_init(&dict, DICT_BENCH_DICT_SIZE, NULL, NULL, NULL, NULL);
#define DICT_BENCH_START(msg)			\
    gettimeofday(&tval_before, NULL);
#define DICT_BENCH_END(msg, ...)					\
    gettimeofday(&tval_after, NULL);					\
    timersub(&tval_after, &tval_before, &tval_result);			\
    printf("\t[");							\
    printf(msg, __VA_ARGS__);						\
    printf("]: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
#define DICT_BENCH_DEINIT()			\
    dict_deinit(&dict)

static NoRet dict_bench_insert(void)
{
    DICT_BENCH_INIT("Insert");
    DICT_BENCH_START();
    for (uint64 i = 0; i < ITER_COUNT; ++i)
        assert(dict_insert(&dict, randstring(KEY_LEN), &i));
    DICT_BENCH_END("Insert %lu entries with the random keys", ITER_COUNT);
    DICT_BENCH_DEINIT();
}

static NoRet dict_bench_replace(void)
{
    DICT_BENCH_INIT("Replace");
    DICT_BENCH_START();
    for (uint64 i = 0; i < ITER_COUNT; ++i)
        dict_replace(&dict, randstring(KEY_LEN), &i);
    DICT_BENCH_END("Replace %lu entries with the random keys", ITER_COUNT);
    DICT_BENCH_DEINIT();
}

static NoRet dict_bench_find(void)
{
    DICT_BENCH_INIT("Find");
    int val = 563;
    for (uint64 i = 0; i < ITER_COUNT; ++i)
        dict_insert(&dict, randstring(KEY_LEN), &val);

    DICT_BENCH_START();
    for (uint64 i = 0; i < ITER_COUNT; ++i)
        dict_find(&dict, randstring(KEY_LEN));
    DICT_BENCH_END("Find %lu entries with the random keys", ITER_COUNT);

    DICT_BENCH_DEINIT();
}

NoRet dict_bench(void)
{
    printf("Dictionary benchmark started\n");
    dict_bench_insert();
    dict_bench_replace();
    dict_bench_find();
    printf("Dictionary benchmark ended\n");
}

#endif /* defined(DICT_BENCH) */

/*********/
/* Tests */
/*********/

#if defined(DICT_TEST)
#include <assert.h>

#define DICT_TESTS_DICT_SIZE 8192

#define DICT_TEST_BEG(name)						\
    char* ___dict_test_name = name;					\
    struct Dictionary dict;						\
    dict_init(&dict, DICT_TESTS_DICT_SIZE, NULL, NULL, NULL, NULL);	\

#define DICT_TEST_END() do {					\
        dict_deinit(&dict);					\
        printf("\t[\"%s\"]: passed\n", ___dict_test_name);	\
    } while (0)

static NoRet dict_test_insert(void)
{
    DICT_TEST_BEG("Insert");

    char* key1 = randstring(8);
    int* val1 = xmalloc(sizeof(int));
    *val1 = 32;

    dict_insert(&dict, key1, val1);
    int* res = dict_find(&dict, key1);
    assert(res);
    assert(*res == *val1);
    assert(dict_find(&dict, key1));
    assert(!dict_insert(&dict, key1, val1));

    DICT_TEST_END();
}

static NoRet dict_test_replace(void)
{
    DICT_TEST_BEG("Replace");
    char* key1 = randstring(8);
    int* val1 = xmalloc(sizeof(*val1));
    int* val2 = xmalloc(sizeof(*val2));
    *val1 = 32;
    *val2 = 54;

    assert(dict_insert(&dict, key1, val1));
    dict_replace(&dict, key1, val2);
    int* res = dict_find(&dict, key1);
    assert(res);
    assert(*res == *val2);
    assert(dict_find(&dict, key1));

    DICT_TEST_END();
}

static NoRet dict_test_find(void)
{
    DICT_TEST_BEG("Find");
    char* key1 = randstring(8);
    int* val1 = xmalloc(sizeof(*val1));
    *val1 = 32;

    assert(dict_insert(&dict, key1, val1));
    int* res = dict_find(&dict, key1);
    assert(res);
    assert(*res == *val1);
    assert(dict_find(&dict, key1));

    DICT_TEST_END();
}

static NoRet dict_test_uniqueness(void)
{
    DICT_TEST_BEG("Uniqueness");

    for (uint64 i = 0; i < (1 << 18); ++i) {
        uint64* val = xmalloc(sizeof(*val));
        *val = i;
        dict_insert(&dict, randstring(8), val);
    }

    printf("\n");
    for (uint64 i = 0; i < dict.size; ++i) {
        for (uint64 j = 0; j < dict.slots[i].size; ++j)
            printf("#");
        printf("\n");
    }

    DICT_TEST_END();
}

NoRet dict_test(void)
{
    printf("Dictionary testing:\n");
    dict_test_insert();
    dict_test_replace();
    dict_test_find();
    /* dict_test_uniqueness(); */
}

#endif /* defined(DICT_TEST) */
