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

#ifndef QUEUE_H
#define QUEUE_H

#include "IntrusiveLinkedList.h"

/**
 * Implementation of FIFO queue structure with circular intrusive doubly
 * linked list.
 */

typedef IntrusiveDListNode QueueNode;
typedef IntrusiveDListNode Queue;

#define queue_init(q) dlist_init(q)

#define queue_enqueue(q, node) dlist_insert_back((q)->prev, node)

#define queue_dequeue(q) dlist_remove_back(q)

#define queue_entry(ptr, type, member) dlist_entry(ptr, type, member)

#define queue_for_each(position, q) dlist_for_each(position, q)

#define queue_size(q) dlist_size(q)

#define queue_is_empty(q) ((q)->prev == (q) && (q)->next == (q))

#define queue_front(q) ((q)->next)

#define queue_destroy(head, type, member, destroy) \
        dlist_destroy(head, type, member, destroy)

#endif /* QUEUE_H */
