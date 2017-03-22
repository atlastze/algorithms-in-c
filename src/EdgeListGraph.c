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

#include <math.h>
#include "EdgeListGraph.h"
#include "DisjointSet.h"

VECTOR_IMPL(edgelist, EdgeNode, EdgeList)

void egraph_init(EdgeListGraph * graph, int isdirected)
{
	graph->vcount = 0;
	graph->isdirected = isdirected;
	edgelist_init(&graph->edges);
}

void egraph_destroy(EdgeListGraph * graph)
{
	graph->vcount = 0;
	edgelist_destroy(&graph->edges);
}

int egraph_set_edge(EdgeListGraph * graph, int i, int start, int end,
		    double weight)
{
	EdgeNode edge;
	edge.start = start;
	edge.end = end;
	edge.weight = weight;
	return edgelist_set(&graph->edges, i, edge);
}

int egraph_add_edge(EdgeListGraph * graph, int start, int end, double weight)
{
	return egraph_set_edge(graph, egraph_vertex_size(graph),
			       start, end, weight);
}

void egraph_read(EdgeListGraph * graph, FILE * fp)
{
	EdgeNode edge;

	int count;
	char s[16];
	if (fscanf(fp, "%d %s\n", &count, s) != 2)
		return;
	if (strcmp(s, "directed") == 0)
		graph->isdirected = 1;
	else if (strcmp(s, "undirected") == 0)
		graph->isdirected = 0;
	else {
		fprintf(stderr, "Unkown graph type!\n");
		return;
	}

	graph->vcount = count;

	while (fscanf(fp, "%d %d %lf\n", &edge.start, &edge.end,
		      &edge.weight) == 3) {
		int i = edge.start, j = edge.end;
		if (i < 0 || i >= count || j < 0 || j >= count) {
			fprintf(stderr, "Vertex index out of range!\n");
			return;
		}
		edgelist_push_back(&graph->edges, edge);
	}
}

void egraph_write(EdgeListGraph * graph, FILE * fp)
{
	fprintf(fp, "the graph in edges list:\n");
	fprintf(fp, "----------------------------------------\n");
	for (int i = 0; i < graph->edges.size; i++) {
		EdgeNode edge;
		edgelist_get(&graph->edges, i, &edge);
		fprintf(fp, "%d\t%d\t%0.2lf\n", edge.start, edge.end,
			edge.weight);
	}
	fprintf(fp, "----------------------------------------\n");
}

void path_resize(EdgeListGraph * path, int source, int vcount)
{
	edgelist_resize(&path->edges, vcount);

	/* Step 1: initialize path */
	for (int i = 0; i < vcount; i++) {
		egraph_set_edge(path, i, -1, i, HUGE_VAL);
	}
	if(source != -1)
		egraph_edge_entry(path, source).weight = 0.0;
}

static inline int compare(EdgeNode edge1, EdgeNode edge2)
{
	if (edge1.weight > edge2.weight)
		return 1;
	if (edge1.weight < edge2.weight)
		return -1;
	return 0;
}

void egraph_kruskal_mst(EdgeListGraph * graph, EdgeListGraph * mst)
{
	DisjointSet set;
	set_init(&set, egraph_vertex_size(graph));

	edgelist_quicksort(&graph->edges, compare);

	int count = 0;
	for (int i = 0; i < graph->edges.size && count < graph->vcount - 1; i++) {
		EdgeNode edge = egraph_edge_entry(graph, i);
		if (set_find(&set, edge.start) != set_find(&set, edge.end)) {
			set_union(&set, edge.start, edge.end);
			edgelist_push_back(&mst->edges, edge);
			count++;
		}
	}
	set_destroy(&set);
}

int egraph_bellman_ford(EdgeListGraph * graph, int source, EdgeListGraph * path)
{
	int n = egraph_vertex_size(graph);

	/* Step 1: initialize path */
	path_resize(path, source, n);

	/* Step 2: relax edges repeatedly */
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < graph->edges.size; j++) {
			int u = egraph_edge_entry(graph, j).start;
			int v = egraph_edge_entry(graph, j).end;
			double w = egraph_edge_entry(graph, j).weight;
			double du = egraph_edge_entry(path, u).weight;
			double dv = egraph_edge_entry(path, v).weight;
			if (dv > du + w) {
				egraph_edge_entry(path, v).start = u;
				egraph_edge_entry(path, v).weight = du + w;
			}
		}
	}

	/* Step 3: check for negative-weight cycles */
	for (int j = 0; j < graph->edges.size; j++) {
		int u = egraph_edge_entry(graph, j).start;
		int v = egraph_edge_entry(graph, j).end;
		double w = egraph_edge_entry(graph, j).weight;
		double du = egraph_edge_entry(path, u).weight;
		double dv = egraph_edge_entry(path, v).weight;
		if (dv > du + w) {
			/* graph contains a negative-weight cycle */
			return 0;
		}
	}
	return 1;
}
