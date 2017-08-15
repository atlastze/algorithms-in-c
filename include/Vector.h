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

#ifndef VECTOR_H
#define VECTOR_H

/**
 * Generic Vector (Dynamic Array) Implementation.
 */

#include <stdlib.h>
#include <string.h>

static const int initialSize = 4;

#define VECTOR_STRUCT_DECL(elementtype, vectortype) \
typedef struct vectortype { \
    elementtype * element; \
    int capacity; \
    int size; \
} vectortype;

#define vector_entry(vector, i) ((vector)->element[i])

/**
 * vector_init - initialize a vector struct.
 * @vector: vector to be initialized
 *
 * Returns 1 on success and 0 on failure to initialize.
 */
#define VECTOR_INIT_DECL(namespace, elementtype, vectortype) \
int namespace ## _init(vectortype *vector)

#define VECTOR_INIT_IMPL(namespace, elementtype, vectortype) \
int namespace ## _init(vectortype *vector) \
{ \
    elementtype *p = (elementtype *)malloc( \
        initialSize*sizeof(elementtype)); \
    if(!p) return 0; \
    memset(p, 0, initialSize*sizeof(elementtype)); \
    vector->element = p; \
    vector->capacity = initialSize; \
    vector->size = 0; \
    return 1; \
}

/**
 * vector_realloc - reallocate memory
 * @vector: vector to be reallocated
 * @capacity: new capacity
 *
 * Returns 1 on success and 0 on failure to reallocate.
 */
#define VECTOR_REALLOC_DECL(namespace, elementtype, vectortype) \
int namespace ## _realloc(vectortype * vector, int capacity)

#define VECTOR_REALLOC_IMPL(namespace, elementtype, vectortype) \
int namespace ## _realloc(vectortype * vector, int capacity) \
{ \
    if(vector->capacity >= capacity) return 1; \
    int m = vector->capacity, n = m; \
    while(n < capacity) n *= 2; \
    elementtype *p = (elementtype *)realloc(vector->element, \
        n*sizeof(elementtype)); \
    if(!p) return 0; \
    /* initialize the newly allocated portion */ \
    memset(p+m, 0, (n-m)*sizeof(elementtype)); \
    vector->element = p; \
    vector->capacity = n; \
    return 1; \
}

/**
 * vector_resize - resize vector
 * @vector: vector to be reallocated
 * @size: new size
 *
 * Returns 1 on success and 0 on failure to reallocate.
 */
#define VECTOR_RESIZE_DECL(namespace, vectortype) \
int namespace ## _resize(vectortype * vector, int size)

#define VECTOR_RESIZE_IMPL(namespace, vectortype) \
int namespace ## _resize(vectortype * vector, int size) \
{ \
    if(vector->size >= size || \
    namespace ## _realloc(vector, size)) \
    { vector->size = size; return 1; } \
    return 0; \
}

#define VECTOR_INDEX_OUT_OF_RANGE_DECL(namespace, vectortype) \
int namespace ## _index_out_of_range(vectortype *vector, int index)

#define VECTOR_INDEX_OUT_OF_RANGE_IMPL(namespace, vectortype) \
int namespace ## _index_out_of_range(vectortype *vector, int index) \
{ \
    if(index < 0 || index >= vector->size) return 1; \
    return 0; \
}

/**
 * vector_from_array - assign elements of a vector with array
 * @vector: vector to be updated
 * @array: array elements
 * @size: array size
 *
 * Returns 1 on success and 0 on failure.
 */
#define VECTOR_FROM_ARRAY_DECL(namespace, elementtype, vectortype) \
int namespace ## _from_array(vectortype * vector, \
    elementtype * array, int size)

#define VECTOR_FROM_ARRAY_IMPL(namespace, elementtype, vectortype) \
int namespace ## _from_array(vectortype * vector, \
    elementtype * array, int size) \
{ \
    if(!namespace ## _resize(vector, size)) return 0; \
    memmove(vector->element, array, size*sizeof(elementtype)); \
    return 1; \
}

/**
 * vector_copy - copy elements of a vector from another vector
 * @dest: vector to be updateed
 * @src: source vector
 *
 * Returns 1 on success and 0 on failure.
 */
#define VECTOR_COPY_DECL(namespace, vectortype) \
int namespace ## _copy(vectortype * dest, vectortype * src)

#define VECTOR_COPY_IMPL(namespace, vectortype) \
int namespace ## _copy(vectortype * dest, vectortype * src) \
{ \
    return namespace ## _from_array(dest, src->element, src->size); \
}

/**
 * vector_multi_set - set multiple elements
 * @dest: vector to be updateed
 * @src: source vector
 *
 * Returns 1 on success and 0 on failure.
 */
#define VECTOR_MULTI_SET_DECL(namespace, elementtype, vectortype) \
int namespace ## _multi_set(vectortype * dest, int start, int end, \
    elementtype element)

#define VECTOR_MULTI_SET_IMPL(namespace, elementtype, vectortype) \
int namespace ## _multi_set(vectortype * dest, int start, int end, \
    elementtype element) \
{ \
    if(namespace ## _index_out_of_range(dest, start)) return 0; \
    if(namespace ## _index_out_of_range(dest, end)) return 0; \
    for(int i = start; i <= end; i++) \
        dest->element[i] = element; \
    return 1; \
}

/**
 * vector_move - batch set elements
 * @dest: vector to be updateed
 * @start1: index from which the dest elements are updated
 * @src: source vector
 * @start2: index from which the src elements is assigned to dest
 * @n: count of elements updated
 *
 * Returns 1 on success and 0 on failure.
 */
#define VECTOR_MOVE_DECL(namespace, elementtype, vectortype) \
int namespace ## _move(vectortype * dest, size_t start1, \
    vectortype * src, size_t start2, size_t n)

#define VECTOR_MOVE_IMPL(namespace, elementtype, vectortype) \
int namespace ## _move(vectortype * dest, size_t start1, \
    vectortype * src, size_t start2, size_t n) \
{ \
    if(namespace ## _index_out_of_range(dest, start1) || \
       namespace ## _index_out_of_range(dest, start1+n-1) || \
       namespace ## _index_out_of_range(src, start2) || \
       namespace ## _index_out_of_range(src, start2+n-1)) \
        return 0; \
    memmove(dest->element+start1, src->element+start2, \
        n*sizeof(elementtype)); \
    return 1; \
}

/**
 * vector_clear - clear all elements.
 * @vector: vector to be cleared
 */
#define VECTOR_CLEAR_DECL(namespace, elementtype, vectortype) \
void namespace ## _clear(vectortype * vector)

#define VECTOR_CLEAR_IMPL(namespace, elementtype, vectortype) \
void namespace ## _clear(vectortype * vector) \
{ \
    namespace ## _resize(vector, 0); \
}

/**
 * vector_destroy - frees the memory allocated.
 * @vector: vector to be destroyed
 */
#define VECTOR_DESTROY_DECL(namespace, vectortype) \
void namespace ## _destroy(vectortype * vector)

#define VECTOR_DESTROY_IMPL(namespace, vectortype) \
void namespace ## _destroy(vectortype * vector) \
{ \
    free(vector->element); \
    vector->element = NULL; \
    vector->size = 0; \
    vector->capacity = 0; \
}

/**
 * vector_get - retrieve element of a vector with index
 * @vector: vector to be retrieved
 * @i: index
 * @element: address of element
 *
 * Returns 1 on success and 0 on out of range.
 */
#define VECTOR_GET_DECL(namespace, elementtype, vectortype) \
int namespace ## _get(vectortype *vector, int i, elementtype * element)

#define VECTOR_GET_IMPL(namespace, elementtype, vectortype) \
int namespace ## _get(vectortype *vector, int i, elementtype * element) \
{ \
    if(namespace ## _index_out_of_range(vector, i)) \
        return 0; \
    *element = vector->element[i]; \
    return 1; \
}

/**
 * vector_set - set element of a vector with index
 * @vector: vector to be updated
 * @i: index
 * @element: the element
 *
 * Returns 1 on success and 0 on failure.
 *
 * NOTE: If @i is out of range, then update vector size with i+1.
 * If @i is out of capacity, then reallocate enough memory.
 * In this case, element pointer of vector maybe changed.
 */
#define VECTOR_SET_DECL(namespace, elementtype, vectortype) \
int namespace ## _set(vectortype *vector, int i, elementtype element)

#define VECTOR_SET_IMPL(namespace, elementtype, vectortype) \
int namespace ## _set(vectortype *vector, int i, elementtype element) \
{ \
    if(namespace ## _index_out_of_range(vector, i)) return 0; \
    vector->element[i] = element; \
    return 1; \
}

/**
 * vector_swap - swap two elements of a vector with indices
 * @vector: vector to be updated
 * @i: index
 * @j: index
 *
 * Returns 1 on success and 0 on index out of range.
 */
#define VECTOR_SWAP_DECL(namespace, elementtype, vectortype) \
int namespace ## _swap(vectortype *vector, int i, int j)

#define VECTOR_SWAP_IMPL(namespace, elementtype, vectortype) \
int namespace ## _swap(vectortype *vector, int i, int j) \
{ \
    if(namespace ## _index_out_of_range(vector, i) || \
       namespace ## _index_out_of_range(vector, j)) \
        return 0; \
    elementtype temp = vector->element[i]; \
    vector->element[i] = vector->element[j]; \
    vector->element[j] = temp; \
    return 1; \
}

/**
 * vector_insert - insert element of a vector with index
 * @vector: vector to be updated
 * @i: index
 * @element: the element
 *
 * Returns 1 on success and 0 on failure.
 *
 * NOTE: If @i is out of range, then update vector size with i+1.
 * If @i is out of capacity, then reallocate enough memory.
 * In this case, element pointer of vector maybe changed.
 */
#define VECTOR_INSERT_DECL(namespace, elementtype, vectortype) \
int namespace ## _insert(vectortype *vector, int i, elementtype element)

#define VECTOR_INSERT_IMPL(namespace, elementtype, vectortype) \
int namespace ## _insert(vectortype *vector, int i, elementtype element) \
{ \
    if(i < 0 || !namespace ## _realloc(vector, vector->size+1)) \
        return 0; \
    memmove(vector->element+i+1, vector->element+i, \
        (vector->size-i)*sizeof(elementtype)); \
    vector->element[i] = element; \
    vector->size++; \
    return 1;     \
 }

/**
 * vector_remove - remove element of a vector with index
 * @vector: vector to be updated
 * @i: index
 *
 * Returns 1 on success and 0 on index out of range.
 */
#define VECTOR_REMOVE_DECL(namespace, elementtype, vectortype) \
int namespace ## _remove(vectortype *vector, int start, int end)

#define VECTOR_REMOVE_IMPL(namespace, elementtype, vectortype) \
int namespace ## _remove(vectortype *vector, int start, int end) \
{ \
    if(namespace ## _index_out_of_range(vector, start) || \
           namespace ## _index_out_of_range(vector, end)) \
        return 0; \
    memmove(vector->element+start, vector->element+end+1, \
        (vector->size-end-1)*sizeof(elementtype)); \
    vector->size -=  end-start+1; \
    return 1;     \
}

/**
 * vector_get_front - retrieve the first element of a vector
 * @vector: vector to be retrieved
 * @element: address of element
 *
 * Returns 1 on success and 0 on out of range.
 */
#define VECTOR_GET_FRONT_DECL(namespace, elementtype, vectortype) \
int namespace ## _get_front(vectortype *vector, elementtype * element)

#define VECTOR_GET_FRONT_IMPL(namespace, elementtype, vectortype) \
int namespace ## _get_front(vectortype *vector, elementtype * element) \
{ \
    return namespace ## _get(vector, 0, element); \
}

/**
 * vector_get_back - retrieve the last element of a vector
 * @vector: vector to be retrieved
 * @element: address of element
 *
 * Returns 1 on success and 0 on out of range.
 */
#define VECTOR_GET_BACK_DECL(namespace, elementtype, vectortype) \
int namespace ## _get_back(vectortype *vector, elementtype * element)

#define VECTOR_GET_BACK_IMPL(namespace, elementtype, vectortype) \
int namespace ## _get_back(vectortype *vector, elementtype * element) \
{ \
    return namespace ## _get(vector, vector->size-1, element); \
}

/**
 * vector_push_front - insert element at the beginning of a vector
 * @vector: vector to be updated
 * @element: the element
 *
 * Returns 1 on success and 0 on failure.
 */
#define VECTOR_PUSH_FRONT_DECL(namespace, elementtype, vectortype) \
int namespace ## _push_front(vectortype *vector, elementtype element)

#define VECTOR_PUSH_FRONT_IMPL(namespace, elementtype, vectortype) \
int namespace ## _push_front(vectortype *vector, elementtype element) \
{ \
    return namespace ## _insert(vector, 0, element); \
}

/**
 * vector_push_front - insert element at the end of a vector
 * @vector: vector to be updated
 * @element: the element
 *
 * Returns 1 on success and 0 on failure.
 */
#define VECTOR_PUSH_BACK_DECL(namespace, elementtype, vectortype) \
int namespace ## _push_back(vectortype *vector, elementtype element)

#define VECTOR_PUSH_BACK_IMPL(namespace, elementtype, vectortype) \
int namespace ## _push_back(vectortype *vector, elementtype element) \
{ \
    return namespace ## _insert(vector, vector->size, element); \
}

/**
 * vector_pop_front - remove the first element of a vector
 * @vector: vector to be updated
 *
 * Returns 1 on success and 0 on index out of range.
 */
#define VECTOR_POP_FRONT_DECL(namespace, vectortype) \
int namespace ## _pop_front(vectortype *vector)

#define VECTOR_POP_FRONT_IMPL(namespace, vectortype) \
int namespace ## _pop_front(vectortype *vector) \
{ \
    return namespace ## _remove(vector, 0, 0); \
}

/**
 * vector_pop_back - remove the last element of a vector
 * @vector: vector to be updated
 *
 * Returns 1 on success and 0 on index out of range.
 */
#define VECTOR_POP_BACK_DECL(namespace, vectortype) \
int namespace ## _pop_back(vectortype *vector)

#define VECTOR_POP_BACK_IMPL(namespace, vectortype) \
int namespace ## _pop_back(vectortype *vector) \
{ \
    return namespace ## _remove(vector, \
        vector->size-1, \
        vector->size-1); \
}

/**
 * _partial_quicksort - sort elements of a vector using quick sort algorithm
 * @array: array to be sorted
 * @startIndex: start index
 * @endIndex: end index
 * @compare: function pointer of elements comparsion
 *
 * NOTE: Quick sort algorithm with Lomuto partition
 * The algorithm maintains the index to put the pivot in variable i and
 * each time it finds an element less than or equal to pivot, this index is
 * incremented and that element would be placed before the pivot.
 */
#define VECTOR_PARTIAL_QUICKSORT_DECL(namespace, elementtype, vectortype) \
static void namespace ## _partial_quicksort(vectortype * vector, \
    int startIndex, int endIndex, \
    int (*compare)(elementtype, elementtype))

#define VECTOR_PARTIAL_QUICKSORT_IMPL(namespace, elementtype, vectortype) \
static void namespace ## _partial_quicksort(vectortype * vector, \
    int startIndex, int endIndex, \
    int (*compare)(elementtype, elementtype)) \
{ \
    if(startIndex < 0 || endIndex < 0 || endIndex - startIndex < 0) \
        return; \
    elementtype pivot = vector->element[endIndex]; \
    int i = startIndex, j; \
    for(j = startIndex; j < endIndex; j++){ \
        if(compare(vector->element[j], pivot) <= 0) { \
            namespace ## _swap(vector, i, j); \
            i++; \
        } \
    } \
    namespace ## _swap(vector, i, endIndex); \
     \
    namespace ## _partial_quicksort(vector, startIndex, i-1, compare); \
    namespace ## _partial_quicksort(vector, i+1, endIndex, compare); \
}

/**
 * vector_quicksort - sort elements of a vector using quick sort algorithm
 * @vector: vector to be sorted
 * @compare: function pointer of elements comparsion
 */
#define VECTOR_QUICKSORT_DECL(namespace, elementtype, vectortype) \
void namespace ## _quicksort(vectortype *vector, \
    int (*compare)(elementtype, elementtype))

#define VECTOR_QUICKSORT_IMPL(namespace, elementtype, vectortype) \
void namespace ## _quicksort(vectortype *vector, \
    int (*compare)(elementtype, elementtype)) \
{ \
    namespace ## _partial_quicksort(vector, \
        0, vector->size-1, compare); \
}

/**
 * vector_find - find element from a vector
 * @vector: vector find from
 * @element: element
 * @compare: function pointer of elements comparsion
 *
 * Returns index on success, or -1 on not found.
 */
#define VECTOR_FIND_DECL(namespace, elementtype, vectortype) \
int namespace ## _find(vectortype *vector, elementtype element, \
    int (*compare)(elementtype, elementtype))

#define VECTOR_FIND_IMPL(namespace, elementtype, vectortype) \
int namespace ## _find(vectortype *vector, elementtype element, \
    int (*compare)(elementtype, elementtype)) \
{ \
    for(int i = 0; i < vector->size; i++) \
        if(compare(vector->element[i], element) == 0) \
            return i; \
    return -1; \
}

#define VECTOR_DECL(namespace, elementtype, vectortype) \
VECTOR_STRUCT_DECL(elementtype, vectortype) \
VECTOR_INIT_DECL(namespace, elementtype, vectortype); \
VECTOR_REALLOC_DECL(namespace, elementtype, vectortype); \
VECTOR_RESIZE_DECL(namespace, vectortype); \
VECTOR_DESTROY_DECL(namespace, vectortype); \
VECTOR_GET_DECL(namespace, elementtype, vectortype); \
VECTOR_SET_DECL(namespace, elementtype, vectortype); \
VECTOR_INSERT_DECL(namespace, elementtype, vectortype); \
VECTOR_REMOVE_DECL(namespace, elementtype, vectortype); \
VECTOR_GET_FRONT_DECL(namespace, elementtype, vectortype); \
VECTOR_GET_BACK_DECL(namespace, elementtype, vectortype); \
VECTOR_PUSH_FRONT_DECL(namespace, elementtype, vectortype); \
VECTOR_PUSH_BACK_DECL(namespace, elementtype, vectortype); \
VECTOR_POP_FRONT_DECL(namespace, vectortype); \
VECTOR_POP_BACK_DECL(namespace, vectortype); \
VECTOR_SWAP_DECL(namespace, elementtype, vectortype); \
VECTOR_PARTIAL_QUICKSORT_DECL(namespace, elementtype, vectortype); \
VECTOR_QUICKSORT_DECL(namespace, elementtype, vectortype); \
VECTOR_FROM_ARRAY_DECL(namespace, elementtype, vectortype); \
VECTOR_COPY_DECL(namespace, vectortype); \
VECTOR_FIND_DECL(namespace, elementtype, vectortype); \
VECTOR_CLEAR_DECL(namespace, elementtype, vectortype); \
VECTOR_MULTI_SET_DECL(namespace, elementtype, vectortype); \
VECTOR_MOVE_DECL(namespace, elementtype, vectortype); \
VECTOR_INDEX_OUT_OF_RANGE_DECL(namespace, vectortype);

#define VECTOR_IMPL(namespace, elementtype, vectortype) \
VECTOR_INIT_IMPL(namespace, elementtype, vectortype) \
VECTOR_REALLOC_IMPL(namespace, elementtype, vectortype) \
VECTOR_RESIZE_IMPL(namespace, vectortype) \
VECTOR_DESTROY_IMPL(namespace, vectortype) \
VECTOR_GET_IMPL(namespace, elementtype, vectortype) \
VECTOR_SET_IMPL(namespace, elementtype, vectortype) \
VECTOR_INSERT_IMPL(namespace, elementtype, vectortype) \
VECTOR_REMOVE_IMPL(namespace, elementtype, vectortype) \
VECTOR_GET_FRONT_IMPL(namespace, elementtype, vectortype) \
VECTOR_GET_BACK_IMPL(namespace, elementtype, vectortype) \
VECTOR_PUSH_FRONT_IMPL(namespace, elementtype, vectortype) \
VECTOR_PUSH_BACK_IMPL(namespace, elementtype, vectortype) \
VECTOR_POP_FRONT_IMPL(namespace, vectortype) \
VECTOR_POP_BACK_IMPL(namespace, vectortype) \
VECTOR_SWAP_IMPL(namespace, elementtype, vectortype) \
VECTOR_PARTIAL_QUICKSORT_IMPL(namespace, elementtype, vectortype) \
VECTOR_QUICKSORT_IMPL(namespace, elementtype, vectortype) \
VECTOR_FROM_ARRAY_IMPL(namespace, elementtype, vectortype) \
VECTOR_COPY_IMPL(namespace, vectortype) \
VECTOR_FIND_IMPL(namespace, elementtype, vectortype) \
VECTOR_CLEAR_IMPL(namespace, elementtype, vectortype) \
VECTOR_MULTI_SET_IMPL(namespace, elementtype, vectortype) \
VECTOR_MOVE_IMPL(namespace, elementtype, vectortype) \
VECTOR_INDEX_OUT_OF_RANGE_IMPL(namespace, vectortype);

#endif /* VECTOR_H */
