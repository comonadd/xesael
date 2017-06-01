/* File: dlinkedlist.c */
/* Creation Date: 2017-01-12*/
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */

#include "dlinkedlist.h"

#include "xmalloc.h"

/***********/
/* Helpers */
/***********/

NoRet dlinkedlist_node_delete(
    struct DLinkedList* list,
    struct DLinkedListNode* node)
{
    list->val_free_f(node->val);
    xfree(node);
}

NoRet dlinked_list_init(
    struct DLinkedList* list,
    NoRet (*val_free_f)(void* val),
    int (*val_cmp_f)(void* val1, void* val2))
{
    list->size = 0;
    list->head = list->sentinel;
    list->tail = list->sentinel;
    list->val_free_f = val_free_f;
    list->val_cmp_f = val_cmp_f;
}

bool dlinkedlist_is_empty(
    struct DLinkedList* list)
{
    return list->head == list->sentinel;
}

struct DLinkedListNode* dlinkedlist_last(
    struct DLinkedList* list)
{
    return
	(list->tail != list->sentinel) ? list->tail : list->head;
}

struct DLinkedListNode* dlinkedlist_first(
    struct DLinkedList* list)
{
    return list->head;
}

/**********/
/* Insert */
/**********/

NoRet dlinkedlist_insert_after(
    struct DLinkedList* list,
    struct DLinkedListNode* node,
    struct DLinkedListNode* new_node)
{
    /* If node is a tail of a list */
    if (node->next == list->sentinel) {
	node->next = new_node;
	new_node->next = list->sentinel;
	new_node->prev = node;
	list->tail = new_node;
	return;
    }
    new_node->next = node->next;
    node->next = new_node;
    new_node->prev = node;
}

NoRet dlinkedlist_insert_before(
    struct DLinkedList* list,
    struct DLinkedListNode* node,
    struct DLinkedListNode* new_node)
{
    /* If node is a head of a list */
    if (node->prev == list->sentinel) {
	node->prev = new_node;
	new_node->prev = list->sentinel;
	new_node->next = node;
	list->head = new_node;
	return;
    }
    node->prev->next = new_node;
    new_node->prev = node->prev;
    new_node->next = node;
    node->prev = new_node;
}

NoRet dlinkedlist_prepend(
    struct DLinkedList* list,
    struct DLinkedListNode* node)
{
    if (dlinkedlist_is_empty(list)) {
	list->head = node;
	list->head->prev = list->sentinel;
	list->head->next = list->sentinel;
    }
    dlinkedlist_insert_before(list, list->head, node);
}

NoRet dlinkedlist_append(
    struct DLinkedList* list,
    struct DLinkedListNode* node)
{
    if (dlinkedlist_is_empty(list)) {
	list->head = node;
	list->head->prev = list->sentinel;
	list->head->next = list->sentinel;
    }
    dlinkedlist_insert_after(list, list->tail, node);
}

/**********/
/* Remove */
/**********/

NoRet dlinkedlist_remove(
    struct DLinkedList* list,
    struct DLinkedListNode* node)
{
    /* If the node that we want to remove is a tail */
    /* of the list */
    if (node->next == list->sentinel) {
	node->prev->next = list->sentinel;
	list->tail = node->prev;
    } else {
	node->prev->next = node->next;
	node->next->prev = node->prev;
    }

    /* If the node that we want to remove is a head */
    /* of the list */
    if (node->prev == list->sentinel) {
	node->next->prev = list->sentinel;
	list->head = node->next;
    } else {
	node->next->prev = node->prev;
	node->prev->next = node->next;
    }

    /* Free memory engaged by the node */
    dlinkedlist_node_delete(list, node);
}

NoRet dlinkedlist_remove_first(
    struct DLinkedList* list)
{
    if (dlinkedlist_is_empty(list)) return;
    dlinkedlist_remove(list, list->head);
}

NoRet dlinkedlist_remove_last(
    struct DLinkedList* list)
{
    if (dlinkedlist_is_empty(list)) return;
    dlinkedlist_remove(list, list->tail);
}

/**********/
/* Search */
/**********/

struct DLinkedListNode* dlinkedlist_search(
    struct DLinkedList* list,
    void* val)
{
    struct DLinkedListNode* node = list->head;
    while (node != list->sentinel) {
	if ((node->val == val) || (list->val_cmp_f(node->val, val) == 0))
	    return node;
	node = node->next;
    }
    return list->sentinel;
}
