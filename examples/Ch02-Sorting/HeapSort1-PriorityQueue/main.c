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

#include <stdio.h>
#include "PriorityQueue.h"
#include "IntegerSequence.h"

HEAP_DECL(heap, Heap, sequence, int, IntegerSequence)
HEAP_IMPL(heap, Heap, sequence, int, IntegerSequence)

/* comapre function used for min-heap */
int compare1(int a, int b)
{
	return a - b;
}

/* comapre function used for max-heap */
int compare2(int a, int b)
{
	return b - a;
}

/* heap construction by insertion */
void create_heap(int *a, int n, int (*compare) (int, int))
{
	printf("\n\n>> Illustrate heap construction step by step:\n");
	IntegerSequence s;
	sequence_init(&s);

	Heap heap;
	heap_init(&heap, &s, compare);

	printf(">> Creating heap by insertion:\n");
	for (int i = 0; i < 10; i++) {
		heap_push(&heap, a[i]);
		printf("   insert %d: ", a[i]);
		sequence_write(stdout, &s, " ");
	}

	sequence_destroy(&s);

	heap_destroy(&heap);
}

/* heap sorting process */
void sort_steps(int *a, int n, int (*compare) (int, int))
{
	printf("\n\n>> Illustrate heap sort step by step:\n");
	printf(">> Creating heap by bottom-up construction...\n");

	IntegerSequence s;
	sequence_init(&s);
	sequence_from_array(&s, a, n);
	printf(">> the original array:\n");
	sequence_write(stdout, &s, " ");

	Heap heap;
	heap_init(&heap, &s, compare);
	printf(">> the heap:\n");
	sequence_write(stdout, &s, " ");

	/* heap sort */
	printf("\n>> heap sorting:\n");
	for (int i = n; i > 0; i--) {
		printf("   peek %d: ", heap_pop(&heap));
		sequence_write(stdout, &s, " ");
	}

	sequence_destroy(&s);

	heap_destroy(&heap);
}

/* call heapsort function */
void sort_inplace(int *a, int n, int (*compare) (int, int))
{
	printf("\n\n>> Illustrate heap sort function usage:\n");
	IntegerSequence s;
	sequence_init(&s);
	sequence_from_array(&s, a, n);
	printf(">> the original array:\n");
	sequence_write(stdout, &s, " ");

	Heap heap;
	heap_init(&heap, &s, compare);
	printf(">> the heap:\n");
	sequence_write(stdout, &s, " ");

	heap_sort(&heap);

	printf(">> the sorted array:\n");
	sequence_write(stdout, &s, " ");

	sequence_destroy(&s);

	heap_destroy(&heap);
}

int main(int argc, char *argv[])
{
	int a[] = { 3, 1, 4, 1, 5, 9, 2, 6, 5, 3 };
	create_heap(a, sizeof(a) / sizeof(int), compare1);

	int b[] = { 4, 1, 3, 2, 16, 9, 10, 14, 8, 7 };
	sort_steps(b, sizeof(b) / sizeof(int), compare1);

	int c[] = { 14, 9, 8, 25, 5, 11, 27, 16, 15, 4, 12, 6, 7, 23, 20 };
	sort_inplace(c, sizeof(c) / sizeof(int), compare1);

	return 0;
}
