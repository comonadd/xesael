/* File: bst.h */
/* Creation Date: 2017-01-16*/
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* Generic binary search tree implementation */

#ifndef BST_H
#define BST_H

#include "types.h"

/**
   @left - left child
   @right - right child
   @key - key of a node
   @val - value of a node
**/
struct BSTNode {
    struct BSTNode* left;
    struct BSTNode* right;
    void* key;
    void* val;
};

/**
   @size - size of a tree
   @root - root node
   @key_cmp_f - function for comparing keys
   @key_free_f - function for freeing keys
   @val_free_f - function for freeing value
**/
struct BST {
    uint64 size;
    struct BSTNode* root;
    int (*key_cmp_f)(const void* key1, const void* key2);
    NoRet (*key_free_f)(void* key);
    NoRet (*val_free_f)(void* val);
};

/***********/
/* Methods */
/***********/

NoRet bst_init(
    struct BST* bst,
    int (*key_cmp_f)(const void* key1, const void* key2),
    NoRet (*key_free_f)(void* key),
    NoRet (*val_free_f)(void* val));

NoRet bst_deinit(struct BST* bst);

struct BST* bst_new(void);

NoRet bst_delete(struct BST* bst);

struct BSTNode* bst_insert_or_get(
    struct BST* bst,
    void* key,
    void* val,
    bool* exists);

bool bst_insert(
    struct BST* bst,
    void* key,
    void* val);

bool bst_remove(
    struct BST* bst,
    void* key);

void* bst_find(
    struct BST const* bst,
    void const* key);

NoRet bst_apply(
    struct BST* bst,
    NoRet (*f)(struct BSTNode* node));

#endif /* BST_H */
