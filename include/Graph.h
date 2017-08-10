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

#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include "IntrusiveLinkedList.h"
#include "Vector.h"
#include "EdgeListGraph.h"
#include "IntegerSequence.h"
#include "DisjointSet.h"
#include "IndexPriorityQueue.h"

/*
 * Aadjacency List Implementation for Graph
 *
 * A representation based entirely on linked lists is called an adjacency list.
 * First, a linked list of all the vertices is constructed. Each of these
 * vertices has a pointer which points to a list of edges originating at that
 * vertex. Since each edge connects a vertex with another vertex, each node in
 * the list also points back to a node in the vertex list corresponding to the
 * other end of the edge. Such a linked representation of a graph is efficient
 * from a space-saving point of view, but as you can see, the representation
 * can produce a complex tangle of pointers. All of this is not really a
 * problem provided that the graph has been constructed properly, but debugging
 * such a tangle of links can be extremely difficult. In addition, many graph
 * algorithms require random access to the set of vertices, and this purely
 * linked version requires a traversal of the vertex list to find any single
 * vertex and its associated edges.
 *
 * Another solution to the implementation problem is to combine arrays and
 * linked lists, in a fashion similar to that used in the chaining method of
 * resolving collisions in a hash table. Here, we define a one-dimensional array
 * in which each element corresponds to a vertex. Each of these elements
 * contains a pointer to a list of edges which originate at that vertex.
 * Such an arrangement still allows us to trace a path through the graph
 * since each entry in the list of edges contains the index number of the vertex
 * which corresponds to the other end of the edge. We can then jump to the
 * corresponding array element and search the list of edges originating from
 * there to continue our traversal of that path.
 */

typedef IntrusiveSListNode EdgeLink;

/**
 * Incomplete types of vertex and edge informations
 * Users should complete them according to the need.
 */
typedef struct _VertexInfo VertexInfo;

typedef struct _Vertex {
	/* Part 1: index of vertex */
	int id;

	/* Part 2: adjacent vertices list head */
	EdgeLink firstedge;

	/* Part 3: vertex state */
	int vertstate;
} Vertex;

typedef struct _Edge {
	/* Part 1: edge weight */
	double weight;

	/* Part 2: adjacent vertex index */
	int neighbor;

	/* Part 3: adjacent vertices list link */
	EdgeLink edgelink;

	/* Part 4: edge state */
	int edgestate;
} Edge;

/**
 * vertices array declaration
 */
VECTOR_DECL(vertexlist, Vertex, VertexList)

/**
 * Graph is defined as a vector of vertices
 */
typedef struct _Graph {
	int isdirected;
	VertexList vertices;
} Graph;

 /* number of vertices */
#define graph_vertex_size(graph) ((graph)->vertices.size)

/* unsafe reference of vertex, we must make sure index is not out of range */
#define graph_vertex_entry(graph, i) (&((graph)->vertices.element[i]))

#define graph_vertex_is_marked(graph, i) \
        (graph_vertex_entry(graph, i)->vertstate == 1)

#define graph_vertex_is_unmarked(graph, i) \
        (graph_vertex_entry(graph, i)->vertstate == 0)

#define graph_mark_vertex(graph, i) \
        (graph_vertex_entry(graph, i)->vertstate = 1)

#define graph_unmark_vertex(graph, i) \
        (graph_vertex_entry(graph, i)->vertstate = 0)

#define graph_mark_vertex_grey(graph, i) \
        (graph_vertex_entry(graph, i)->vertstate = 2)

#define graph_vertex_is_grey(graph, i) \
        (graph_vertex_entry(graph, i)->vertstate == 2)

#define graph_mark_vertex_black(graph, i) \
        (graph_vertex_entry(graph, i)->vertstate = 1)

#define graph_vertex_is_black(graph, i) \
        (graph_vertex_entry(graph, i)->vertstate == 1)

/* convert intrusive singly linked list node to edge */
#define graph_edge_entry(pe) slist_entry(pe, Edge, edgelink)

/* convert intrusive singly linked list position to edge */
#define graph_edge_entry_of_position(pe) graph_edge_entry((pe)->next)

/**
 * graph_init - initialize a graph
 * @graph: graph to be initialized.
 * @isdirected: directed graph flag
 */
void graph_init(Graph * graph, int isdirected);

/**
 * graph_add_vertex - add a vertex to a graph
 * @graph: graph to be inserted
 */
void graph_add_vertex(Graph * graph);

/**
 * graph_add_edge - add an edge(directed graph) or
 * two edges(undirected graph) to a graph
 * @graph: graph to be updated
 * @i: the souce vertex index
 * @j: the destination vertex index
 * @weight: edge weight
 */
void graph_add_edge(Graph * graph, int i, int j, double weight);

/**
 * graph_get_edge - get edge position
 * @graph: graph to be searched
 * @i: the souce vertex index
 * @j: the destination vertex index
 *
 * Returns the adjacent list postion on success, NULL on failure.
 */
EdgeLink *graph_get_edge(Graph * graph, int i, int j);

/**
 * graph_remove_edge - remove edges of two endpoints
 * @graph: graph to be updated
 * @i: the souce vertex index
 * @j: the destination vertex index
 */
void graph_remove_edge(Graph * graph, int i, int j);

/**
 * graph_destroy - destroy a graph
 * @graph: the graph to be destroyed
 */
void graph_destroy(Graph * graph);

/**
 * graph_write - display graph in the form of adjacency list
 * @graph: graph to be displayed
 */
void graph_write(FILE * fp, Graph * graph);

/**
 * graph_read - read data from file
 * @graph: the graph to be populated
 * @fp: file pointer, the file includes lines of edge information(endpoints and
 *      weight), for example:
 *      8 directed
 *	0	1	12.0
 *	1	3	9.0
 *	1	2	15.0
 *	...
 */
void graph_read(FILE * fp, Graph * graph);

/**
 * graph_reset_vertices - reset vertices state
 * @graph: the graph to be reset
 */
void graph_reset_vertices(Graph * graph);

/**
 * graph_reset_edges - reset edges state
 * @graph: the graph to be reset
 */
void graph_reset_edges(Graph * graph);

/**
 * graph_depth_first_order - depth first searching graph
 * @graph: the graph to be searched
 * @i: the start vertex index
 * @preorder: depth-first-search preorder
 * @postorder: depth-first-search postorder
 *
 * NOTE: If @preorder or @postorder is not needed, pass NULL.
 */
void graph_depth_first_order(Graph * graph, int i, IntegerSequence * preorder,
	                         IntegerSequence * postorder);

/**
 * graph_depth_first_search - depth first searching graph
 * @graph: the graph to be searched
 * @i: the start vertex index
 * @s: depth-first-search vertices order（preorder）
 */
void graph_depth_first_search(Graph * graph, int i, IntegerSequence * s);

/**
 * graph_breadth_first_search - breadth first searching graph
 * @graph: the graph to be searched
 * @i: the start vertex index
 * @s: breadth-first-search vertices order
 */
void graph_breadth_first_search(Graph * graph, int i, IntegerSequence * s);

/**
 * graph_depth_first_path - find path with depth first search algorithm
 * @graph: the graph to be searched
 * @i: the start vertex index
 * @path: stores path when return
 */
void graph_depth_first_path(Graph * graph, int i, EdgeListGraph * path);

/**
 * graph_breadth_first_path - find path with breadth first search algorithm
 * @graph: the graph to be searched
 * @i: the start vertex index
 * @path: stores path when return
 */
void graph_breadth_first_path(Graph * graph, int i, EdgeListGraph * path);

/**
 * graph_connected_components - find connected components with
 * depth-first-search algorithm
 * @graph: graph to be processed
 * @s: component sequence separated with -1
 *
 * Returns components number
 */
int graph_connected_components(Graph * graph, IntegerSequence * s);

/**
 * graph_topological_sort - topological sort of digraphs with
 * depth-first-search algorithm
 * @graph: graph to be processed
 * @s: sequence of vertices indices
 *
 * Returns 1 on success, 0 on failure
 */
int graph_topological_sort(Graph * graph, IntegerSequence * s);

/**
 * graph_reverse - reverse digraph
 */
void graph_reverse(Graph * src, Graph * dest);

/**
 * graph_kosaraju_scc - Kosaraju's algorithm for strongly connected components
 */
int graph_kosaraju_scc(Graph * graph, IntegerSequence * s);

/**
 * graph_tarjan_scc - Tarjan's algorithm for strongly connected components
 */
int graph_tarjan_scc(Graph * graph, IntegerSequence * s);

/**
 * graph_prim_mst - Prim's algorithm of minimum spanning tree(MST)
 * @graph: the graph to be searched
 * @path: stores edges of MST when return
 */
void graph_prim_linear_search(Graph * graph, EdgeListGraph * path);
void graph_prim_priority_queue(Graph * graph, EdgeListGraph * path);
void graph_prim(Graph * graph, EdgeListGraph * path);

/**
 * graph_dijkstra - Dijkstra's algorithm of single source-source shortest-paths
 * @graph: the graph to be searched
 * @source: start vertex index;
 * @path: stores edges when return
 *
 * NOTE: Dijkstra's algorithm solves the single-source shortest-paths problem
 * in edge-weighted digraphs with nonnegative weights using extra space
 * proportional to V and time proportional to E log V (in the worst case).
 */
void graph_dijkstra_linear_search(Graph * graph, int source,
				  EdgeListGraph * path);
void graph_dijkstra_priority_queue(Graph * graph, int source,
				   EdgeListGraph * path);
void graph_dijkstra(Graph * graph, int source, EdgeListGraph * path);

/**
* graph_bellman_ford - Bellman-Ford's algorithm of single source-source
* shortest-paths
* @graph: the graph to be searched
* @source: start vertex index;
* @path: stores edges when return
*
* NOTE: The Bellman-Ford algorithm solves the single-source shortest-paths
* problem from a given source s (or finds a negative cycle reachable from s)
* for any edge-weighted digraph with V vertices and E edges, in time
* proportional to E*V and extra space proportional to V, in the worst case.
*/
int graph_bellman_ford(Graph * dgraph, int source, EdgeListGraph * path);

/**
 * graph_create - construct adjacency list from edge list
 */
void graph_create(EdgeListGraph * src, Graph * dest);

/**
 * graph_critical_path - find critical path of AOE
 * @graph: edge-weighted DAG
 * @source: source vertex
 * @path: the longest path, which is the critical path
 */
void graph_critical_path(Graph * graph, int source, EdgeListGraph * path);

#endif /* GRAPH_H */
