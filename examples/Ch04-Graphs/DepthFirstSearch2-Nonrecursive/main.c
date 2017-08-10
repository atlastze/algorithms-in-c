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

#include "Graph.h"
#include <stdio.h>
#include <string.h>

/**
 * find the first unvisited neighbor of vertex v
 */
static inline int graph_unvisited_neighbor(Graph * graph, int i)
{
	slist_for_each(pe, &graph_vertex_entry(graph, i)->firstedge) {
		Edge *edge = graph_edge_entry_of_position(pe);
		if (graph_vertex_entry(graph, edge->neighbor)->vertstate == 0) {
			return edge->neighbor;
		}
	}
	return -1;
}

/**
 * Non-recursive implementation of depth first search
 * http://www.mathcs.emory.edu/~cheung/Courses/323/Syllabus/Graph/dfs.html
 */
void graph_nonrecursive_dfs(Graph * graph, int i, IntegerSequence * s)
{
	IntegerSequence stack;	/* stack of vertices */
	sequence_init(&stack);

	sequence_push_front(&stack, i);
	graph_vertex_entry(graph, i)->vertstate = 1;
	sequence_push_back(s, i);

	while (stack.size != 0) {
		sequence_get_front(&stack, &i);

		/* find first unmarked adjacent vertex */
		int neighbor = graph_unvisited_neighbor(graph, i);

		if (neighbor != -1) {
			sequence_push_front(&stack, neighbor);
			sequence_push_back(s, neighbor);
			graph_vertex_entry(graph, neighbor)->vertstate = 1;
		} else
			sequence_pop_front(&stack);
	}
	sequence_destroy(&stack);
}

int main(int argc, char *argv[])
{
	if (argc < 2)
		return 0;

	Graph graph;
	graph_init(&graph, 0);

	FILE *fp = fopen(argv[1], "r");
	if (!fp)
		exit(1);

	graph_read(fp, &graph);
	fclose(fp);

	graph_write(stdout, &graph);

	IntegerSequence s;
	sequence_init(&s);

	graph_reset_vertices(&graph);
	graph_nonrecursive_dfs(&graph, 0, &s);
	printf("\nThe depth-first-search order from vertex 0:\n");
	sequence_write(stdout, &s, " ");

	sequence_destroy(&s);

	/* destroy graph */
	graph_destroy(&graph);

	return 0;
}
