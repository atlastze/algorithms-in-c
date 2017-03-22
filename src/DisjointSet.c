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

#include "DisjointSet.h"

void set_init(DisjointSet * set, int count)
{
	sequence_init(&set->parent);
	sequence_init(&set->rank);
	set->count = count;

	for (int i = 0; i < count; i++) {
		sequence_push_back(&set->parent, i);
		sequence_push_back(&set->rank, 1);
	}
}

void set_destroy(DisjointSet * set)
{
	sequence_destroy(&set->parent);
	sequence_destroy(&set->rank);
}

int set_find(DisjointSet * set, int i)
{
	/* follow links to find a root */
	while (set_parent(set, i) != i)
		i = set_parent(set, i);
	return i;
}

int set_union(DisjointSet * set, int i, int j)
{
	i = set_find(set, i);
	j = set_find(set, j);
	if (i == j)
		return i;

	set->count--;
	/* make smaller root point to larger one */
	if (set_rank(set, i) < set_rank(set, j)) {
		set_rank(set, j) += set_rank(set, i);
		return set_parent(set, i) = j;
	} else {
		set_rank(set, i) += set_rank(set, j);
		return set_parent(set, j) = i;
	}
}

void set_read(DisjointSet * set, FILE * fp)
{
	int i, j;
	while (fscanf(fp, "%d %d", &i, &j) == 2) {
		/* add new elements if necessary */
		int size = (i > j ? i : j) + 1;
		for (int k = set_size(set); k < size; k++) {
			sequence_push_back(&set->parent, k);
			sequence_push_back(&set->rank, 1);
			set->count++;
		}

		set_union(set, i, j);
	}
}

void set_write(DisjointSet * set, FILE * fp)
{
	for (int i = 0; i < set_size(set); i++) {
		fprintf(fp, "%d\t%d\n", i, set_parent(set, i));
	}
}
