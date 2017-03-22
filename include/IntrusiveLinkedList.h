/* Copyright (C) 2017, kylinsage <kylinsage@gmail.com>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef INTRUSIVE_LINKED_LIST_H
#define INTRUSIVE_LINKED_LIST_H

#include <stdlib.h>

/* Offset of member MEMBER in a struct of type TYPE. */
#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

/*
 * Circular Intrusive Doubly Linked List Implementation.
 * The next reference inside a node can be viewed as a link or pointer to
 * another node.  We define that the POSITION of current node is the pointer to
 * the previous node. We can traverse the list starting at the head. The tail
 * node is a special node, where the next pointer is always pointing or linking
 * to a null reference, indicating the end of the list.
 * Dummy header is used because they help with the implementation.
 */

typedef struct _IntrusiveDListNode {
	struct _IntrusiveDListNode *prev;
	struct _IntrusiveDListNode *next;

} IntrusiveDListNode;

typedef IntrusiveDListNode IntrusiveDList;

/**
 * dlist_entry - get the struct for this entry
 * @ptr:	the &struct list_head pointer.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_struct within the struct.
 */
#define dlist_entry(ptr, type, member) \
	((type *)((char *)(ptr)-offsetof(type, member)))

/**
 * dlist_for_each - iterate over a list
 * @position:  used as a loop cursor.
 * @head: the head for your list.

 * NOTE:  We can change list elements of a list when use this macro, but
 * we should not change links or pointers of the list.
 */
#define dlist_for_each(position, head) \
	for(IntrusiveDListNode *position = (head); \
	position->next != (head); \
	position = position->next)

/**
 * dlist_init - initialize a list
 * @head: the head for your list.
 */
static inline void dlist_init(IntrusiveDList * head)
{
	head->prev = head->next = head;
}

/**
 * dlist_insert_back - insert a node at a specific position
 * @position: list position to add it after
 * @element: pointer to element
 */
void dlist_insert_back(IntrusiveDListNode * position,
		       IntrusiveDListNode * node);

/**
 * dlist_remove - remove a node at a specific position
 * @position: list position to remove it after
 *
 * NOTE: The node is deleted and retruns the pointer to element.
 */
void dlist_remove_back(IntrusiveDListNode * position);

/**
 * dlist_size - count size of list nodes, not including dummy head
 * @head: list to be counted
 */
static inline size_t dlist_size(IntrusiveDList * head)
{
	size_t count = 0;
	dlist_for_each(position, head) {
		count++;
	}
	return count;
}

/**
 * dlist_destroy - destroy list node containers, not including dummy head
 * @head: list to be destroyed
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_struct within the struct.
 * @destroy:    the destructor of the struct wherein link is embedded.
 *
 * NOTE: It is the responsibilities of programmers to mamage the memory of
 * dummy head.
 */
#define dlist_destroy(head, type, member, destroy)                         \
{                                                                          \
	IntrusiveDListNode *position = head;                               \
	while(position->next != (head)) {                                  \
		type *element = dlist_entry(position->next, type, member); \
		dlist_remove_back(position);                               \
		destroy(element);                                          \
	}                                                                  \
}

/*
 * Intrusive Singly Linked List Implementation.
 * The next reference inside a node can be viewed as a link or pointer to
 * another node.  We define that the POSITION of current node is the pointer to
 * the previous node. We can traverse the list starting at the head. The tail
 * node is a special node, where the next pointer is always pointing or linking
 * to a null reference, indicating the end of the list.
 * Dummy header is used because they help with the implementation.
 */

typedef struct _IntrusiveSListNode {
	struct _IntrusiveSListNode *next;

} IntrusiveSListNode;

typedef IntrusiveSListNode IntrusiveSList;

/**
 * slist_entry - get the struct for this entry
 * @ptr:	the &struct list node pointer.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_struct within the struct.
 */
#define slist_entry(ptr, type, member) \
	((type *)((char *)(ptr)-offsetof(type, member)))

/**
 * slist_entry_of_position - get the struct for this entry
 * @position:	the &struct list position (pointer to previous node).
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_struct within the struct.
 */
#define slist_entry_of_position(position, type, member) \
	slist_entry(position->next, type, member)

/**
 * slist_for_each - iterate over a list
 * @position:  used as a loop cursor.
 * @head: the head for your list.

 * NOTE:  We can change list elements of a list when use this macro, but
 * we should not change links or pointers of the list.
 */
#define slist_for_each(position, head) \
	for(IntrusiveSListNode *position = (head); \
	position->next != NULL; \
	position = position->next)

/**
 * slist_init - initialize a list
 * @head: the head for your list.
 */
static inline void slist_init(IntrusiveSList * head)
{
	head->next = NULL;
}

/**
 * slist_insert_back - insert a node at a specific position
 * @position: list position to add it after
 * @element: pointer to element
 */
void slist_insert_back(IntrusiveSListNode * position,
		       IntrusiveSListNode * node);

/**
 * slist_remove - remove a node at a specific position
 * @position: list position to remove it after
 *
 * NOTE: The node is deleted and retruns the pointer to element.
 */
void slist_remove_back(IntrusiveSListNode * position);

/**
 * slist_size - count size of list nodes, not including dummy head
 * @head: list to be counted
 */
static inline size_t slist_size(IntrusiveSList * head)
{
	size_t count = 0;
	slist_for_each(position, head) {
		count++;
	}
	return count;
}

/**
 * slist_destroy - destroy list node containers, not including dummy head
 * @head: list to be destroyed
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_struct within the struct.
 * @destroy:    the destructor of the struct wherein link is embedded.
 *
 * NOTE: It is the responsibilities of programmers to mamage the memory of
 * dummy head.
 */
#define slist_destroy(head, type, member, destroy)                         \
{                                                                          \
	IntrusiveSListNode *position = head;                               \
	while(position->next) {                                            \
		type *element = slist_entry(position->next, type, member); \
		slist_remove_back(position);                               \
		destroy(element);                                          \
	}                                                                  \
}

#endif /* INTRUSIVE_LINKED_LIST_H */
