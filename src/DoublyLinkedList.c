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

#include "DoublyLinkedList.h"

void dlist_insert_back(DoublyLinkedListNode * position, void *element)
{
    DoublyLinkedListNode *node = dlist_create_node(element);

    /* link node and position->next */
    node->next = position->next;
    position->next->prev = node;

    /* linke position and node */
    position->next = node;
    node->prev = position;
}

void *dlist_remove_back(DoublyLinkedListNode * position)
{
    if (!position->next)
        return NULL;

    DoublyLinkedListNode *node = position->next;
    position->next = node->next;
    node->next->prev = position;

    void *element = node->element;
    free(node);
    return element;
}

DoublyLinkedListNode *dlist_find(DoublyLinkedList * head, void *element,
                                 Comparator compare)
{
    dlist_for_each(position, head) {
        if (position->next->element &&
            compare(position->next->element, element) == 0)
            return position;
    }
    return NULL;
}

void dlist_destroy(DoublyLinkedList * head, Destructor destroy)
{
    while (head->next != head) {
        void *element = dlist_remove_back(head);
        if (destroy)
            destroy(element);
    }
}
