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

#ifndef DISJOINT_SET_H
#define DISJOINT_SET_H

#include <stdio.h>
#include "IntegerSequence.h"

typedef struct _DisjointSet {
	IntegerSequence parent;	/* parent link - site indexed */
	IntegerSequence rank;	/* size of component for roots - site indexed */
	int count;		/* number of components */
} DisjointSet;

#define set_parent(set, i) (set->parent.element[i])

#define set_size(set) (set->parent.size)

#define set_rank(set, i) (set->rank.element[i])

void set_init(DisjointSet * set, int count);

void set_destroy(DisjointSet * set);

int set_find(DisjointSet * set, int i);

int set_union(DisjointSet * set, int i, int j);

void set_read(DisjointSet * set, FILE * fp);

void set_write(DisjointSet * set, FILE * fp);

#endif /* DISJOINT_SET_H */
