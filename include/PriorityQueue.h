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

#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include "Vector.h"

/**
 * Implementation of Priority Queue with Heap (Binary Heap)
 *
 * In computer science, a heap is a specialized tree-based data structure that
 * satisfies the heap property: If A is a parent node of B, then the key
 * (the value) of node A is ordered with respect to the key of node B with the
 * same ordering applying across the heap. A heap can be classified further as
 * either a "max heap" or a "min heap". In a max heap, the keys of parent nodes
 * are always greater than or equal to those of the children and the highest
 * key is in the root node. In a min heap, the keys of parent nodes are less
 * than or equal to those of the children and the lowest key is in the root
 * node. (from Wikipedia)
 *
 * Properties:
 * if a[0 ... n-1] is a heap, then:
 * 1. if 0 <= 2*i+1 <= n-1, then compare(a[i], a[2*i+1]) <=0; (left child)
 * 2. if 0 <= 2*i+2 <= n-1, then compare(a[i], a[2*i+2]) <=0; (right child)
 */

/**
 * Heap Data Structure
 * CAUTION: The heap and keys share the same elements, please pay attention
 * to the modification of elements by using functions of either heap and
 * vector.
 */
#define HEAP_STRUCT_DECL(heaptype, elementtype, vectortype) \
typedef struct heaptype { \
    vectortype *keys; \
    int heapsize; \
    int (*compare)(elementtype, elementtype); \
} heaptype;

#define heap_entry(heap, i) ((heap)->keys->element[i])

#define heap_size(heap) ((heap)->heapsize)

#define keys_size(heap) ((heap)->keys->element.size)

#define heap_isempty(heap) ((heap)->heapsize == 0)

#define heap_compare(heap, i, j) ((heap)->compare(heap_entry(heap, i), \
    heap_entry(heap, j)))

/**
 * heap_sift_up -- bottom-up reheapify
 * NOTE: The nodes from 0th to (n-1)th are heap-ordered, and the n-th nodes
 * violates heaporder, the function is to restore heaporder by exchange the
 * node with its parent until to the root of the heap.
 */
#define HEAP_SIFT_UP_DECL(heap_namespace, heaptype, vector_namespace) \
void heap_namespace ## _sift_up(heaptype * heap, int n)

#define HEAP_SIFT_UP_IMPL(heap_namespace, heaptype, vector_namespace) \
void heap_namespace ## _sift_up(heaptype * heap, int n) \
{  \
    int child = n; \
    while(child >= 1) { \
        int parent = (child-1)/2; \
        if(heap_compare(heap, child, parent) >= 0) \
            break; \
        vector_namespace ## _swap(heap->keys, parent, child); \
        child = parent; \
    } \
}

/**
 * heap_sift_down -- create a subheap
 * @vector -- the elements vector
 * @n -- the root of the subheap to be created
 *
 * NOTE: The reheapifing process is down to the last node of the heap. After
 * the process, the subheap nodes (n-th, (2n+1)-th, (2n+2)-th, ...) are
 * in heap-ordered.
 */
#define HEAP_SIFT_DOWN_DECL(heap_namespace, heaptype, vector_namespace) \
void heap_namespace ## _sift_down(heaptype * heap, int n)

#define HEAP_SIFT_DOWN_IMPL(heap_namespace, heaptype, vector_namespace) \
void heap_namespace ## _sift_down(heaptype * heap, int n) \
{  \
    int parent = n; \
    while(2*parent+1 <= heap->heapsize-1) { /* at least one child */ \
        int child = 2*parent+1; \
        /* two children, exchange the node with the smaller one */ \
        if(child+1 <= heap->heapsize-1 && heap_compare(heap, \
        child, child+1) > 0) \
            child = child+1; \
        if(heap_compare(heap, parent, child) > 0) { \
            vector_namespace ## _swap(heap->keys, \
                parent, child); \
            parent = child; \
        } else \
            break; \
    } \
}

/**
 * heap_init -- bottom_up heap construction, heapify an array by
 * creating subheaps from size/2 down to 0
 */
#define HEAP_INIT_DECL(heap_namespace, heaptype, elementtype, vectortype) \
void heap_namespace ## _init(heaptype * heap, vectortype * keys, \
    int (*compare)(elementtype, elementtype))

#define HEAP_INIT_IMPL(heap_namespace, heaptype, elementtype, vectortype) \
void heap_namespace ## _init(heaptype * heap, vectortype * keys, \
    int (*compare)(elementtype, elementtype)) \
{  \
    heap->keys = keys; heap->heapsize = keys->size; \
    heap->compare = compare; \
    for(int i = keys->size/2; i >= 0; i--) \
        heap_namespace ## _sift_down(heap, i); \
}

/**
 * heap_destroy -- detach keys
 *
 * NOTE: the keys is just detached from the heap, and not destroyed.
 */
#define HEAP_DESTROY_DECL(heap_namespace, heaptype, vectortype) \
void heap_namespace ## _destroy(heaptype * heap)

#define HEAP_DESTROY_IMPL(heap_namespace, heaptype, vectortype) \
void heap_namespace ## _destroy(heaptype * heap) \
{  \
    heap->keys = NULL; \
    heap->heapsize = 0; \
    heap->compare = NULL; \
}

/**
 * heap_insert -- insert an element into a heap
 */
#define HEAP_PUSH_DECL(heap_namespace, heaptype, vector_namespace, elementtype) \
void heap_namespace ## _push(heaptype * heap, elementtype element)

#define HEAP_PUSH_IMPL(heap_namespace, heaptype, vector_namespace, elementtype) \
void heap_namespace ## _push(heaptype * heap, elementtype element) \
{  \
    vector_namespace ## _insert(heap->keys, heap->heapsize, element); \
    heap_namespace ## _sift_up(heap, heap->heapsize); \
    heap->heapsize++; \
}

/**
 * heap_top -- return the first node of a heap, but doesn't pop it
 */
#define HEAP_TOP_DECL(heap_namespace, heaptype, vector_namespace, elementtype) \
elementtype heap_namespace ## _top(heaptype * heap)

#define HEAP_TOP_IMPL(heap_namespace, heaptype, vector_namespace, elementtype) \
elementtype heap_namespace ## _top(heaptype * heap) \
{  \
    if(heap_isempty(heap)) { \
        fprintf(stderr, "heap is empty!\n"); \
        exit(1); \
    } \
    return  heap_entry(heap, 0); \
}

/**
 * heap_pop -- swap the first and last nodes of a heap, and then
 * top-down reheapify the nodes of the heap except for the last node.
 */
#define HEAP_POP_DECL(heap_namespace, heaptype, vector_namespace, elementtype) \
elementtype heap_namespace ## _pop(heaptype * heap)

/* swap 0 and ith element, and top-down bottom_up */
#define HEAP_POP_IMPL(heap_namespace, heaptype, vector_namespace, elementtype) \
elementtype heap_namespace ## _pop(heaptype * heap) \
{  \
    if(heap_isempty(heap)) { \
        fprintf(stderr, "heap is empty!\n"); \
        exit(1); \
    } \
    elementtype element = heap_entry(heap, 0); \
    vector_namespace ## _swap(heap->keys, 0, heap->heapsize-1); \
    heap->heapsize--; \
    heap_namespace ## _sift_down(heap, 0); \
    return element; \
}

/**
 * heap_update -- update a heap node and restore heap order
 */
#define HEAP_UPDATE_DECL(heap_namespace, heaptype, vector_namespace, elementtype) \
void heap_namespace ## _update(heaptype * heap, int i, elementtype element)

#define HEAP_UPDATE_IMPL(heap_namespace, heaptype, vector_namespace, elementtype) \
void heap_namespace ## _update(heaptype * heap, int i, elementtype element) \
{ \
    vector_namespace ## _set(heap->keys, i, element); \
    if(i > 0 && heap_compare(heap, (i-1)/2, i) > 0) \
        heap_namespace ## _sift_up(heap, i); \
    else \
        heap_namespace ## _sift_down(heap, i); \
}

/**
 * heap_sort -- implement heap sorting
 */
#define HEAP_SORT_DECL(heap_namespace, heaptype) \
void heap_namespace ## _sort(heaptype * heap)

#define HEAP_SORT_IMPL(heap_namespace, heaptype) \
void heap_namespace ## _sort(heaptype * heap) \
{ \
    for( int i = heap->heapsize; i > 0; i--) \
        heap_namespace ## _pop(heap); \
}

#define HEAP_DECL(heap_namespace, heaptype, vector_namespace, elementtype, vectortype) \
HEAP_STRUCT_DECL(heaptype, elementtype, vectortype) \
HEAP_SIFT_UP_DECL(heap_namespace, heaptype, vector_namespace); \
HEAP_SIFT_DOWN_DECL(heap_namespace, heaptype, vector_namespace); \
HEAP_INIT_DECL(heap_namespace, heaptype, elementtype, vectortype); \
HEAP_DESTROY_DECL(heap_namespace, heaptype, vectortype); \
HEAP_PUSH_DECL(heap_namespace, heaptype, vector_namespace, elementtype); \
HEAP_POP_DECL(heap_namespace, heaptype, vector_namespace, elementtype); \
HEAP_TOP_DECL(heap_namespace, heaptype, vector_namespace, elementtype); \
HEAP_UPDATE_DECL(heap_namespace, heaptype, vector_namespace, elementtype); \
HEAP_SORT_DECL(heap_namespace, heaptype);

#define HEAP_IMPL(heap_namespace, heaptype, vector_namespace, elementtype, vectortype) \
HEAP_SIFT_UP_IMPL(heap_namespace, heaptype, vector_namespace) \
HEAP_SIFT_DOWN_IMPL(heap_namespace, heaptype, vector_namespace) \
HEAP_INIT_IMPL(heap_namespace, heaptype, elementtype, vectortype) \
HEAP_DESTROY_IMPL(heap_namespace, heaptype, vectortype) \
HEAP_PUSH_IMPL(heap_namespace, heaptype, vector_namespace, elementtype) \
HEAP_POP_IMPL(heap_namespace, heaptype, vector_namespace, elementtype) \
HEAP_TOP_IMPL(heap_namespace, heaptype, vector_namespace, elementtype) \
HEAP_UPDATE_IMPL(heap_namespace, heaptype, vector_namespace, elementtype) \
HEAP_SORT_IMPL(heap_namespace, heaptype)

#endif /* PRIORITY_QUEUE_H */
