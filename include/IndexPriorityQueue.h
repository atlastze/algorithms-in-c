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
#include "IntegerSequence.h"

/**
 * Implementation of Index Priority Queue with Heap (Binary Heap)
 *
 * The data structure supports increase-key or decrease-key operations, that is
 * updating a key within a max- or min-heap, respectively. The binary heap here
 * maintains an array of indices that maps from keys to their positions in the
 * binary heap, and also an arary of indices that maps form binary heap to their
 * positions in the keys.
 *
 * Applications:
 *      Dijkstra's algorithm for single source shortest path
 *      Prim's algorithm for minimum spanning tree
 *
 * -- from Robert Sedgewick, Kevin Wayne, Algorithms 4th/e
 *
 * See also:
 * 'Adaptable Prirority Queue' in Data Structures and Algorithms in C++/Java
 * by Goodrich, Tamassia and Mount
 */

/**
 * Heap Data Structure
 * CAUTION: The heap and keys share the same elements, please pay attention
 * to the modification of elements by using functions of either heap and keys.
 */
#define HEAP_STRUCT_DECL(heaptype, elementtype, vectortype) \
typedef struct heaptype { \
    vectortype *keys; \
    IntegerSequence h2k; \
    IntegerSequence k2h; \
    int heapsize; \
    int (*compare)(elementtype, elementtype); \
} heaptype;

#define heap_entry(heap, i) ((heap)->keys->element[(heap)->h2k.element[i]])

#define heap_size(heap) ((heap)->heapsize)

#define keys_size(heap) ((heap)->keys->size)

#define heap_isempty(heap) ((heap)->heapsize == 0)

#define heap_compare(heap, i, j) ((heap)->compare(heap_entry(heap, i), \
    heap_entry(heap, j)))

#define HEAP_SWAP_DECL(heap_namespace, heaptype) \
void heap_namespace ## _swap(heaptype * heap, int i, int j)

#define HEAP_SWAP_IMPL(heap_namespace, heaptype) \
void heap_namespace ## _swap(heaptype * heap, int i, int j) \
{ \
    int k1 = vector_entry(&heap->h2k, i); \
    int k2 = vector_entry(&heap->h2k, j); \
    /* update heap-keys indices */ \
    sequence_swap(&heap->h2k, i, j); \
    /* update keys-heap indices */ \
    sequence_set(&heap->k2h, k1, j); \
    sequence_set(&heap->k2h, k2, i); \
}

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
        heap_namespace ## _swap(heap, parent, child); \
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
            heap_namespace ## _swap(heap, parent, child); \
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
    sequence_init(&heap->h2k); \
    sequence_init(&heap->k2h); \
    heap->compare = compare; \
    for(int i = 0; i < keys->size; i++) { \
        sequence_insert(&heap->h2k, i, i); \
        sequence_insert(&heap->k2h, i, i); \
    } \
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
    sequence_destroy(&heap->h2k); \
    sequence_destroy(&heap->k2h); \
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
    sequence_insert(&heap->h2k, heap->heapsize, heap->heapsize); \
    sequence_insert(&heap->k2h, heap->heapsize, heap->heapsize); \
    heap_namespace ## _sift_up(heap, heap->heapsize); \
    heap->heapsize++; \
}

/**
 * heap_top -- return the first node of a heap, but doesn't pop it
 * Returns kyes' index.
 */
#define HEAP_TOP_DECL(heap_namespace, heaptype, vector_namespace, elementtype) \
int heap_namespace ## _top(heaptype * heap)

#define HEAP_TOP_IMPL(heap_namespace, heaptype, vector_namespace, elementtype) \
int heap_namespace ## _top(heaptype * heap) \
{  \
    if(heap_isempty(heap)) { \
        fprintf(stderr, "heap is empty!\n"); \
        exit(1); \
    } \
    return  vector_entry(&heap->h2k, 0); \
}

/**
 * heap_pop -- swap the first and last nodes of a heap, and then
 * top-down reheapify the nodes of the heap except for the last node.
 */
#define HEAP_POP_DECL(heap_namespace, heaptype, vector_namespace, elementtype) \
int heap_namespace ## _pop(heaptype * heap)

/* swap 0 and ith element, and top-down bottom_up */
#define HEAP_POP_IMPL(heap_namespace, heaptype, vector_namespace, elementtype) \
int heap_namespace ## _pop(heaptype * heap) \
{  \
    if(heap_isempty(heap)) { \
        fprintf(stderr, "heap is empty!\n"); \
        exit(1); \
    } \
    int keyIndex = vector_entry(&heap->h2k, 0); \
    heap_namespace ## _swap(heap, 0, heap->heapsize-1); \
    heap->heapsize--; \
    heap_namespace ## _sift_down(heap, 0); \
    return keyIndex; \
}

/**
 * heap_update -- update a keys node and restore heap order
 * @heap: of which the keys will be updated
 * @i: the index of keys to be updated
 *
 * NOTE: We just update keys and restore heap order of indices
 */
#define HEAP_UPDATE_DECL(heap_namespace, heaptype, vector_namespace, elementtype) \
void heap_namespace ## _update(heaptype * heap, int i)

#define HEAP_UPDATE_IMPL(heap_namespace, heaptype, vector_namespace, elementtype) \
void heap_namespace ## _update(heaptype * heap, int i) \
{ \
    /* convert key index to heap index */ \
    i = vector_entry(&heap->k2h, i); \
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
HEAP_SWAP_DECL(heap_namespace, heaptype); \
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
HEAP_SWAP_IMPL(heap_namespace, heaptype) \
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
