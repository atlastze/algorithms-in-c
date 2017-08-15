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

#ifndef EDGELIST_GRAPH_H
#define EDGELIST_GRAPH_H

#include <stdio.h>
#include "Vector.h"
#include "GraphReader.h"

/**
 * Edge List Structure of Graph
 *
 * The edge list is the simplest and most common implementation of the edge
 * collection is by using a list.
 * The main feature of the edge list structure is that it provides direct
 * access from edges to the vertices they are incident upon.
 * One method that is inefficient for the edge list structure is that of
 * accessing the edges that are incident upon a vertex. Determining this set
 * of vertices requires an exhaustive inspection of all the edge objects in
 * the edge collection.
 * Thus, the edge list representation is simple but has significant limitations.
 * -- Michael T. Goodrich, et al. Data Structures and Algorithms in C++, 2ed
 */

typedef struct _EdgeNode {
    int start;                  /* start vertex index  */
    int end;                    /* end vertex index    */
    double weight;              /* weight of the edge  */
} EdgeNode;

VECTOR_DECL(edgelist, EdgeNode, EdgeList)

typedef struct _EdgeListGraph {
    int vcount;                 /* count of vertices   */
    int isdirected;             /* directed graph flag */
    EdgeList edges;             /* array of edges      */
} EdgeListGraph;

#define egraph_edge_entry(graph, i) (graph->edges.element[i])

#define egraph_edge_size(graph) (graph->edges.size)

#define egraph_vertex_size(graph) (graph->vcount)

/*#define egraph_set_edge(graph, i, node) edgelist_set(&(graph)->edges, i, node)*/

#define egraph_clear(graph) {edgelist_clear(&(graph)->edges); (graph)->vcount = 0;}

void egraph_init(EdgeListGraph * graph, int isdirected);

void egraph_destroy(EdgeListGraph * graph);

int egraph_set_edge(EdgeListGraph * graph, int i, int start, int end,
                    double weight);

int egraph_add_edge(EdgeListGraph * graph, int start, int end, double weight);

void egraph_read(FILE * fp, EdgeListGraph * graph);

void egraph_write(FILE * fp, EdgeListGraph * graph);

/**
 * path_resize - initialize paths
 * @path: paths to be resized
 * @source: source vertex, if no source, pass -1
 * @vcount: vertices count
 */
void path_resize(EdgeListGraph * path, int source, int vcount);

/**
 * Kruskal's algorthm for minimum spanning tree
 */
void egraph_kruskal_mst(EdgeListGraph * graph, EdgeListGraph * mst);

/**
 * NOTE: The Bellman-Ford algorithm solves the single-source shortest-paths
 * problem from a given source s (or finds a negative cycle reachable from s)
 * for any edge-weighted digraph with V vertices and E edges, in time
 * proportional to E*V and extra space proportional to V, in the worst case.
 */
int egraph_bellman_ford(EdgeListGraph * graph, int source,
                        EdgeListGraph * path);

#endif /* EDGELIST_GRAPH_H */
