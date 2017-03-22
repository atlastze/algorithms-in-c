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

#ifndef STACK_H
#define STACK_H

#include "IntrusiveLinkedList.h"

/**
 * Implementation of LIFO stack structure with circular intrusive singly
 * linked list.
 */

typedef IntrusiveSListNode StackNode;
typedef IntrusiveSListNode Stack;

#define stack_init(s) slist_init(s)

#define stack_push(s, node) slist_insert_back(s, node)

#define stack_pop(s) slist_remove_back(s)

#define stack_entry(ptr, type, member) slist_entry(ptr, type, member)

#define stack_for_each(position, s) slist_for_each(position, s)

#define stack_size(s) slist_size(s)

#define stack_is_empty(s) ((s)->next == NULL)

#define stack_top(s) ((s)->next)

#define stack_destroy(s, type, member, destroy) \
        slist_destroy(s, type, member, destroy)

#endif /* STACK_H */
