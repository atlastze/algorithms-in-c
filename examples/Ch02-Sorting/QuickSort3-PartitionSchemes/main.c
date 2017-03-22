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
#include <time.h>
#include <stdlib.h>
#include "IntegerSequence.h"
#include "StopWatch.h"

/* Quick sort algorithm with Hoare partition scheme */
void quicksort1(IntegerSequence * a, int startIndex, int endIndex)
{
	if (startIndex < 0 || endIndex < 0 || endIndex - startIndex <= 0)
		return;
	int pivot = vector_entry(a, startIndex);
	int i, j;
	for (i = startIndex, j = endIndex;; i++, j--) {
		while (vector_entry(a, i) < pivot)
			i++;
		while (vector_entry(a, j) > pivot)
			j--;

		if (i >= j)
			break;

		sequence_swap(a, i, j);
	}
	sequence_swap(a, startIndex, j);

	quicksort1(a, startIndex, j - 1);
	quicksort1(a, j + 1, endIndex);
}

/* Quick sort algorithm with Lomuto partition scheme */
void quicksort2(IntegerSequence * a, int startIndex, int endIndex)
{
	if (startIndex < 0 || endIndex < 0 || endIndex - startIndex <= 0)
		return;
	int pivot = vector_entry(a, endIndex);
	int i = startIndex;	/* place for swapping */
	int j;
	for (j = startIndex; j < endIndex; j++) {
		if (vector_entry(a, j) <= pivot) {
			sequence_swap(a, i, j);
			i++;
		}
	}
	sequence_swap(a, i, endIndex);

	quicksort2(a, startIndex, i - 1);
	quicksort2(a, i + 1, endIndex);
}

int compare(int a, int b)
{
	return a - b;
}

int main(int argc, char *argv[])
{
	IntegerSequence a, b, c;

	/* initialize vector, default capacity: 4 */
	sequence_init(&a);
	sequence_init(&b);
	sequence_init(&c);

	srand(time(NULL));
	int n = 200;
	while (n < 1000000) {
		sequence_clear(&a);
		sequence_clear(&b);
		sequence_clear(&c);
		/* populate vector with random numbers */
		for (int i = 0; i < n; i++)
			sequence_push_back(&c, rand());

		sequence_copy(&a, &c);
		sequence_copy(&b, &c);
		/* quick sort */
		double t0, t1;
		double dt1, dt2, dt3;

		t0 = real_time();
		quicksort1(&b, 0, n - 1);
		t1 = real_time();
		dt1 = t1 - t0;

		t0 = real_time();
		quicksort2(&a, 0, n - 1);
		t1 = real_time();
		dt2 = t1 - t0;

		t0 = real_time();
		/*sequence_quicksort(&c, compare); */
		quicksort2(&c, 0, n - 1);
		t1 = real_time();
		dt3 = t1 - t0;

		printf("n: %7d, dt2/dt1: %lf, dt3/dt2: %lf\n", n, dt2 / dt1,
		       dt3 / dt2);
		/*print(&v); */
		n *= 2;
	}

	sequence_destroy(&a);
	sequence_destroy(&b);
	sequence_destroy(&c);

	return 0;
}
