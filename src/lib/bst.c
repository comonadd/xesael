/* File: bst.c */
/* Creation Date: 2017-01-16*/
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* Generic binary search tree implementation */

#include "bst.h"

#include "types.h"
#include "macros.h"
#include "xmalloc.h"

/***********/
/* Helpers */
/***********/

static struct BSTNode* bst_node_new(
    void* key,
    void* val)
{
    struct BSTNode* node = xmalloc(sizeof(*node));
    node->key = key;
    node->val = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static NoRet bst_node_delete(
    struct BST* bst,
    struct BSTNode* node)
{
    bst->key_free_f(node->key);
    bst->val_free_f(node->val);
    xfree(node);
}

/***********/
/* Methods */
/***********/

NoRet bst_init(
    struct BST* bst,
    int (*key_cmp_f)(const void* key1, const void* key2),
    NoRet (*key_free_f)(void* key),
    NoRet (*val_free_f)(void* val))
{
    bst->size = 0;
    bst->root = NULL;
    bst->key_cmp_f = key_cmp_f;
    bst->key_free_f = key_free_f;
    bst->val_free_f = val_free_f;
}

NoRet bst_deinit(struct BST* bst)
{
    if (!bst->root) return;
    bst_node_delete(bst, bst->root);
}

NoRet bst_delete(struct BST* bst)
{
    bst_deinit(bst);
    xfree(bst);
}

struct BSTNode* bst_insert_or_get(
    struct BST* bst,
    void* key,
    void* val,
    bool* exists)
{
    *exists = false;

    if (!bst->root) {
	bst->root = bst_node_new(key, val);
	bst->size = 1;
	return bst->root;
    }

    /* Start traverse from the root node */
    struct BSTNode* node = bst->root;

    /* While node is not NULL */
    while (node) {
	/* If key that we wanna to insert is */
	/* equal to the current node's key */
	if ((key == node->key) || (bst->key_cmp_f(key, node->key) == 0)) {
	    /* Such key already exists */
	    *exists = true;
	    return node;
	}

	/* If key that we wanna to insert is */
	/* "greater" than the current node's key */
	if (bst->key_cmp_f(key, node->key) > 0) {
	    if (!node->right) {
		/* If there is no right child */
		/* We found a place to insert a new node */
		++bst->size;
		node->right = bst_node_new(key, val);
		return node;
	    }
	    /* Go to the right node */
	    node = node->right;
	} else {
	    if (!node->left) {
		/* If there is no left child */
		/* We found a place to insert a new node */
		++bst->size;
		node->left = bst_node_new(key, val);
		return node;
	    }
	    /* Go to the left node */
	    node = node->left;
	}
    }

    /* Impossible */
    return false;
}

bool bst_insert(
    struct BST* bst,
    void* key,
    void* val)
{
    bool exists = false;
    bst_insert_or_get(bst, key, val, &exists);
    return !exists;
}

bool bst_remove(
    struct BST* bst,
    void* key)
{
    struct BSTNode* parent = NULL;
    struct BSTNode* node = bst->root;
    while (node) {
	/* If keys are equal */
	if ((key == node->key) || (bst->key_cmp_f(key, node->key) == 0)) {
	    /* Found a node to remove */
	    --bst->size;
	    if (!parent) {
		bst_node_delete(bst, node);
		return true;
	    }

	    if (bst->key_cmp_f(key, parent->key) > 0) {
		bst_node_delete(bst, node);
		parent->right = NULL;
	    } else {
		bst_node_delete(bst, node);
		parent->left = NULL;
	    }
	    return true;
	}

	/* Save the parent */
	parent = node;

	/* Go to the next node */
	if (bst->key_cmp_f(key, node->key) > 0) {
	    node = node->right;
	} else {
	    node = node->left;
	}
    }
    /* There is no node with given key */
    return false;
}

void* bst_find(
    struct BST const* bst,
    void const* key)
{
    /* Start search from the root node */
    struct BSTNode* node = bst->root;

    /* While node is not NULL */
    while (node) {
	/* If keys are equal */
	if ((key == node->key) || (bst->key_cmp_f(key, node->key) == 0)) {
	    /* We found it! */
	    return node->val;
	}

	/* Go to the next node */
	if (bst->key_cmp_f(key, node->key) > 0) {
	    node = node->right;
	} else {
	    node = node->left;
	}
    }

    /* Not found */
    return false;
}

static NoRet bst_node_apply(
    struct BSTNode* node,
    NoRet (*f)(struct BSTNode* node))
{
    f(node);
    if (node->left) bst_node_apply(node->left, f);
    if (node->right) bst_node_apply(node->right, f);
}

NoRet bst_apply(
    struct BST* bst,
    NoRet (*f)(struct BSTNode* node))
{
    bst_node_apply(bst->root, f);
}
