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

#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <stdlib.h>
#include "Common.h"

/*
 * Circular Doubly Linked List Implementation.
 * The next reference inside a node can be viewed as a link or pointer to
 * another node. We define that the POSITION of current node is the pointer to
 * the previous node. We can traverse the list starting at the head. The tail
 * node is a special node, where the next pointer is always pointing or linking
 * to a null reference, indicating the end of the list.
 * Dummy header is used because they help with the implementation.
 */

typedef struct _DoublyLinkedListNode {
	void *element;
	struct _DoublyLinkedListNode *prev;
	struct _DoublyLinkedListNode *next;

} DoublyLinkedListNode;

typedef DoublyLinkedListNode DoublyLinkedList;

/**
 * dlist_for_each - iterate over a list
 * @position:  used as a loop cursor.
 * @head: the head for your list.

 * NOTE:  We can change list elements of a list when use this macro, but
 * we should not change links or pointers of the list.
 */
#define dlist_for_each(position, head) \
	for(DoublyLinkedListNode *position = (head); \
	position->next != (head); \
	position = position->next)

/**
 * dlist_create_node - create node with a specific element
 * @element: pointer to element
 *
 * Returns the node newly created
 */
static inline DoublyLinkedListNode *dlist_create_node(void *element)
{
	DoublyLinkedListNode *node =
	    (DoublyLinkedListNode *) malloc(sizeof(DoublyLinkedListNode));
	node->element = element;
	node->prev = NULL;
	node->next = NULL;
	return node;
}

/**
 * dlist_init - initialize a list
 * @head: the head for your list.
 */
static inline void dlist_init(DoublyLinkedList * head)
{
	head->element = NULL;
	head->prev = head->next = head;
}

/**
 * dlist_insert_back - insert a node at a specific position
 * @position: list position to add it after
 * @element: pointer to element
 */
void dlist_insert_back(DoublyLinkedListNode * position, void *element);

/**
 * dlist_remove - remove a node at a specific position
 * @position: list position to remove it after
 *
 * NOTE: The node is deleted and retruns the pointer to element.
 */
void *dlist_remove_back(DoublyLinkedListNode * position);

/**
 * dlist_find - find the first element in a list
 * @head: the head for your list.
 * @elemen: pointer to element
 * @compare: compare function of specific element type
 *
 * Returns the position of the node (i.e. the previous node)
 */
DoublyLinkedListNode *dlist_find(DoublyLinkedList * head, void *element,
				 Comparator compare);

/**
 * dlist_destroy - destroy a list
 * @head: the head for your list.
 * @destroy: destroy function of specific element type
 *
 * NOTE: It is the responsibilities of programmers to mamaged the memory of elements.
 * If destroy is NULL, then the memory of elements will not be freed. At the end of
 * this function, only the dummy head is left.
 */
void dlist_destroy(DoublyLinkedList * head, Destructor destroy);

#endif /* DOUBLY_LINKED_LIST_H */
