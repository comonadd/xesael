/* File: dlinkedlist.h */
/* Creation Date: 2017-01-12*/
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* Generic implementation of double linked list */

#ifndef DLINKEDLIST_H
#define DLINKEDLIST_H

#include "lib/types.h"

/**
   $ Description:
   #   The double-linked list node
   @val - value of the node
   @next - pointer to a next node
   @prev - pointer to a previous node
**/
struct DLinkedListNode {
  void* val;
  struct DLinkedListNode* next;
  struct DLinkedListNode* prev;
};

/**
   $ Description:
   #   The actual double-linked list structure
   #   Properties:
   #   If list is empty:
   #     - head is NULL
   #     - tail is NULL
   #   If list has one element in it:
   #     - head is that element
   #     - tail is NULL
   #   If list has two elements in it:
   #     - head is first element
   #     - tail is second element
   #   - If there is no a head - head is the sentinel
   #   - If there is no a tail - tail is the sentinel
   #   - List head previous is always the sentinel node
   #   - List tail next is always the sentinel node
   #   - If node has no next or prev node, they will be pointing
   #     to a sentinel node
   #   - List head is always the first item in the list
   #   - List tail is always the last item in the list
   @size - count of items in the list
   @sentinel - terminal node
   @head - first item in the list
   @tail - last item in the list
   @eq_f - function that says "are two values of the nodes are equal"
**/
struct DLinkedList {
  uint64 size;
  struct DLinkedListNode* sentinel;
  struct DLinkedListNode* head;
  struct DLinkedListNode* tail;
  NoRet (*val_free_f)(void* val);
  int (*val_cmp_f)(void* val1, void* val2);
};

/***********/
/* Helpers */
/***********/

NoRet dlinkedlist_node_delete(struct DLinkedList* list,
                              struct DLinkedListNode* node);

/**
   $ Description:
   #   This function initializes the
   #   double-linked list structure
**/
NoRet dlinked_list_init(struct DLinkedList* list,
                        NoRet (*val_free_f)(void* val),
                        int (*val_cmp_f)(void* val1, void* val2));

/**
   $ Description:
   #   This function returns true
   #   if the list is empty, false otherwise
**/
bool dlinked_list_is_empty(struct DLinkedList* list);

/**
   $ Description:
   #   This function returns first element of
   #   a list
   $ Return value:
   #   If first element exists, returns it
   #   If not, returns sentinel node
**/
struct DLinkedListNode* dlinkedlist_first(struct DLinkedList* list);

/**
   $ Description:
   #   This function returns last element of
   #   a list
   $ Return value:
   #   If last element exists, returns it
   #   If not, returns sentinel node
**/
/* NOTE: This function should be ALWAYS used to retreive */
/*       the last element of the list (if list has no tail, it returns head) */
struct DLinkedListNode* dlinkedlist_last(struct DLinkedList* list);

/**********/
/* Insert */
/**********/

NoRet dlinkedlist_insert_after(struct DLinkedList* list,
                               struct DLinkedListNode* node,
                               struct DLinkedListNode* new_node);

NoRet dlinkedlist_insert_before(struct DLinkedList* list,
                                struct DLinkedListNode* node,
                                struct DLinkedListNode* new_node);

/**
   $ Description:
   #   Insert to the beginning of the list
**/
NoRet dlinkedlist_prepend(struct DLinkedList* list,
                          struct DLinkedListNode* node);

/**
   $ Description:
   #   Insert to the end of the list
**/
NoRet dlinkedlist_append(struct DLinkedList* list,
                         struct DLinkedListNode* node);

/**********/
/* Remove */
/**********/

NoRet dlinkedlist_remove(struct DLinkedList* list,
                         struct DLinkedListNode* node);

NoRet dlinkedlist_remove_first(struct DLinkedList* list);

NoRet dlinkedlist_remove_last(struct DLinkedList* list);

/**********/
/* Search */
/**********/

struct DLinkedListNode* dlinkedlist_search(struct DLinkedList* list, void* val);

#endif /* DLINKEDLIST_H */
