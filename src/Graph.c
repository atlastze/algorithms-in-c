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

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "Graph.h"

VECTOR_IMPL(vertexlist, Vertex, VertexList)
HEAP_DECL(heap, Heap, edgelist, EdgeNode, EdgeList)
HEAP_IMPL(heap, Heap, edgelist, EdgeNode, EdgeList)

#define _min(i, j) ((i) < (j) ? (i) : (j))

int weight_compare(EdgeNode e1, EdgeNode e2)
{
	if (e1.weight > e2.weight)
		return 1;
	if (e1.weight < e2.weight)
		return -1;
	return 0;
}

void graph_init(Graph * graph, int isdirected)
{
	graph->isdirected = isdirected;
	vertexlist_init(&graph->vertices);
}

void graph_add_vertex(Graph * graph)
{
	Vertex vertex;
	vertex.id = graph_vertex_size(graph);
	vertex.vertstate = 0;
	slist_init(&vertex.firstedge);
	vertexlist_push_back(&graph->vertices, vertex);
}

static inline void
graph_add_unidirect_edge(Graph * graph, int i, int j, double weight)
{
	if (i < 0 || j < 0 || i >= graph_vertex_size(graph)
	    || j >= graph_vertex_size(graph))
		return;

	Edge *edge;

	edge = (Edge *) malloc(sizeof(Edge));
	edge->weight = weight;
	edge->neighbor = j;
	edge->edgestate = 0;
	slist_insert_back(&graph_vertex_entry(graph, i)->firstedge,
			  &edge->edgelink);
}

static inline void
graph_add_bidirect_edge(Graph * graph, int i, int j, double weight)
{
	graph_add_unidirect_edge(graph, i, j, weight);
	graph_add_unidirect_edge(graph, j, i, weight);
}

void graph_add_edge(Graph * graph, int i, int j, double weight)
{
	if (graph->isdirected)
		graph_add_unidirect_edge(graph, i, j, weight);
	else
		graph_add_bidirect_edge(graph, i, j, weight);
}

EdgeLink *graph_get_edge(Graph * graph, int i, int j)
{
	slist_for_each(position, &graph_vertex_entry(graph, i)->firstedge) {
		if (graph_edge_entry_of_position(position)->neighbor == j) {
			return position;
		}
	}
	return NULL;
}

void graph_remove_edge(Graph * graph, int i, int j)
{
	EdgeLink *position;

	position = graph_get_edge(graph, i, j);
	if (position) {
		EdgeLink *pe = position->next;
		slist_remove_back(position);
		free(graph_edge_entry(pe));
	}
}

void graph_destroy(Graph * graph)
{
	for (int i = 0; i < graph_vertex_size(graph); i++) {
		Vertex *pv = graph_vertex_entry(graph, i);
		slist_destroy(&pv->firstedge, Edge, edgelink, free);
	}
	vertexlist_destroy(&graph->vertices);
}

void graph_write(Graph * graph, FILE * fp)
{
	fprintf(fp, "The graph represented as adjacency list:\n");
	fprintf(fp, "----------------------------------------\n");
	for (int i = 0; i < graph_vertex_size(graph); i++) {
		Vertex *vertex = graph_vertex_entry(graph, i);
		fprintf(fp, "v%-2d:", vertex->id);
		slist_for_each(pe, &vertex->firstedge) {
			Vertex *neighbor = graph_vertex_entry(graph,
				graph_edge_entry_of_position(pe)->neighbor);
			fprintf(fp, " -- [v%-2d, %0.2lf]", neighbor->id,
				graph_edge_entry_of_position(pe)->weight);
		}
		fprintf(fp, "\n");
	}
	fprintf(fp, "----------------------------------------\n");
}

void graph_read(Graph * graph, FILE * fp)
{
	int i, j;
	double weight;

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

	for (int k = 0; k < count; k++) {
		graph_add_vertex(graph);
	}

	while (fscanf(fp, "%d %d %lf\n", &i, &j, &weight) == 3) {
		if (i < 0 || i >= count || j < 0 || j >= count) {
			fprintf(stderr, "Vertex index out of range!\n");
			return;
		}
		graph_add_edge(graph, i, j, weight);
	}
}

void graph_reset_vertices(Graph * graph)
{
	for (int i = 0; i < graph_vertex_size(graph); i++) {
		graph_unmark_vertex(graph, i);
	}
}

void graph_reset_edges(Graph * graph)
{
	for (int i = 0; i < graph_vertex_size(graph); i++) {
		slist_for_each(pe, &graph_vertex_entry(graph, i)->firstedge) {
			graph_edge_entry_of_position(pe)->edgestate = 0;
		}
	}
}

/*******************************************************************************
 * Recursive implementation of depth first search
 ******************************************************************************/
void graph_depth_first_order(Graph * graph, int i, IntegerSequence * preorder,
	                         IntegerSequence * postorder)
{
	graph_mark_vertex(graph, i);
	if(preorder) sequence_push_back(preorder, i);
	slist_for_each(pe, &graph_vertex_entry(graph, i)->firstedge) {
		Edge *edge = graph_edge_entry_of_position(pe);

		if (graph_vertex_is_unmarked(graph, edge->neighbor))
			graph_depth_first_order(graph, edge->neighbor, preorder, postorder);
	}
	if(postorder) sequence_push_back(postorder, i);
}

void graph_depth_first_search(Graph * graph, int i, IntegerSequence * s)
{
	graph_depth_first_order(graph, i, s, NULL);
}

/*******************************************************************************
 * Finding paths with depth first search
 ******************************************************************************/
void _graph_depth_first_path(Graph * graph, int i, EdgeListGraph * path)
{
	graph_mark_vertex(graph, i);
	slist_for_each(pe, &graph_vertex_entry(graph, i)->firstedge) {
		Edge *edge = graph_edge_entry_of_position(pe);

		if (graph_vertex_is_unmarked(graph, edge->neighbor)) {
			egraph_edge_entry(path, edge->neighbor).start = i;
			egraph_edge_entry(path, edge->neighbor).weight = edge->weight;
			_graph_depth_first_path(graph, edge->neighbor, path);
		}
	}
}

void graph_depth_first_path(Graph * graph, int i, EdgeListGraph * path)
{
	graph_reset_vertices(graph);
	path_resize(path, i, graph_vertex_size(graph));
	_graph_depth_first_path(graph, i, path);
}

/*******************************************************************************
 * Implementation of breadth first search
 ******************************************************************************/
void graph_breadth_first_search(Graph * graph, int i, IntegerSequence * s)
{
	IntegerSequence queue;	/* queue of vertices */
	sequence_init(&queue);

	sequence_push_back(&queue, i);
	graph_mark_vertex(graph, i);

	while (queue.size != 0) {
		sequence_get_front(&queue, &i);
		sequence_push_back(s, i);
		sequence_pop_front(&queue);

		slist_for_each(pe, &graph_vertex_entry(graph, i)->firstedge) {
			Edge *edge = graph_edge_entry_of_position(pe);
			if (graph_vertex_is_unmarked(graph, edge->neighbor)) {
				sequence_push_back(&queue, edge->neighbor);
				graph_mark_vertex(graph, edge->neighbor);
			}
		}
	}
	sequence_destroy(&queue);
}

/*******************************************************************************
 * Finding paths with breadth first search
 ******************************************************************************/
void graph_breadth_first_path(Graph * graph, int i, EdgeListGraph * path)
{
	graph_reset_vertices(graph);
	path_resize(path, i, graph_vertex_size(graph));

	IntegerSequence queue;	/* queue of vertices */
	sequence_init(&queue);

	sequence_push_back(&queue, i);
	graph_mark_vertex(graph, i);

	while (queue.size != 0) {
		sequence_get_front(&queue, &i);
		sequence_pop_front(&queue);

		slist_for_each(pe, &graph_vertex_entry(graph, i)->firstedge) {
			Edge *edge = graph_edge_entry_of_position(pe);
			if (graph_vertex_is_unmarked(graph, edge->neighbor)) {
				egraph_edge_entry(path, edge->neighbor).start = i;
				egraph_edge_entry(path, edge->neighbor).weight = edge->weight;
				sequence_push_back(&queue, edge->neighbor);
				graph_mark_vertex(graph, edge->neighbor);
			}
		}
	}
	sequence_destroy(&queue);
}

/*******************************************************************************
 * Find connected components with depth-first-search algorithm
 ******************************************************************************/
int graph_connected_components(Graph * graph, IntegerSequence * s)
{
	graph_reset_vertices(graph);
	int count = 0;
	for(int i = 0; i < graph_vertex_size(graph); i++)
	{
		if(graph_vertex_is_unmarked(graph, i)) {
			count ++;
			graph_depth_first_search(graph, i, s);
			sequence_push_back(s, -1);
		}
	}
	return count;
}

/*******************************************************************************
 * Tpological sort of digraphs with depth-first-search algorithm
 ******************************************************************************/
/**
 * Topological sort helper function, do:
 *     1. depth-first-search postorder
 *     2. detect cycle
 *
 * Returns 1 on acyclic digraphs, otherwise, return 0.
 */
int _graph_topological_sort(Graph * graph, int i, IntegerSequence * s)
{
	graph_mark_vertex_grey(graph, i);
	slist_for_each(pe, &graph_vertex_entry(graph, i)->firstedge) {
		Edge *edge = graph_edge_entry_of_position(pe);

		if (graph_vertex_is_unmarked(graph, edge->neighbor))
			_graph_topological_sort(graph, edge->neighbor, s);
		else if (graph_vertex_is_grey(graph, edge->neighbor))
			return 0;
	}
	sequence_push_front(s, i);
	graph_mark_vertex_black(graph, i);
	return 1;
}

int graph_topological_sort(Graph * graph, IntegerSequence * s)
{
	graph_reset_vertices(graph);
	for (int i = 0; i < graph_vertex_size(graph); i++) {
		if (graph_vertex_is_unmarked(graph, i)) {
			if (!_graph_topological_sort(graph, i, s))
				return 0;
		}
	}
	return 1;
}

/*******************************************************************************
 * Kosaraju's algorithm for strongly connected components
 ******************************************************************************/
void graph_reverse(Graph * src, Graph * dest)
{
	int n = graph_vertex_size(src);
	for (int i = 0; i < n; i++) {
		graph_add_vertex(dest);
	}
	for (int i = 0; i < n; i++) {
		slist_for_each(pe, &graph_vertex_entry(src, i)->firstedge) {
			Edge *edge = graph_edge_entry_of_position(pe);
			graph_add_edge(dest, edge->neighbor, i, edge->weight);
		}
	}
}

int graph_kosaraju_scc(Graph * graph, IntegerSequence * s)
{
	int n = graph_vertex_size(graph);

	Graph gt; /* reverse of graph */
	graph_init(&gt, graph->isdirected);
	graph_reverse(graph, &gt);

	IntegerSequence postorder;
	sequence_init(&postorder);

	graph_reset_vertices(&gt);
	for(int i = 0; i < n; i++) {
		if(graph_vertex_is_unmarked(&gt, i))
			graph_depth_first_order(&gt, i, NULL, &postorder);
	}

	graph_reset_vertices(graph);
	int count = 0;
	for(int i = n-1; i >= 0; i--) {
		int start = vector_entry(&postorder, i);
		if(graph_vertex_is_unmarked(graph, start)) {
			count ++;
			graph_depth_first_search(graph, start, s);
			sequence_push_back(s, -1);
		}
	}

	graph_destroy(&gt);
	sequence_destroy(&postorder);
	return count;
}

/*******************************************************************************
 * Tarjan's algorithm for strongly connected components
 * https://activities.tjhsst.edu/sct/lectures/1516/SCT_Tarjans_Algorithm.pdf
 ******************************************************************************/
/**
 * Tarjan SCC hepler function
 * order[i]: depth-first-search order
 * link[i]: minimum of the following:
 *          1. order[i]
 *          2. link[j], if (i, j) is tree edge and i is the parent node
 *          3. order[j], if (i, j) is backward edge
 */
/**
 * Infomations used for Tarjan's algorithm
 */
typedef struct _TarjanInfo {
	int number;
	int count;
	IntegerSequence order;
	IntegerSequence link;
	IntegerSequence stack;
} TarjanInfo;

/**
 * Initialize informations used Tarjan's algorithm
 * @n: graph vertices count
 */
void _tarjan_init(TarjanInfo * info, int n)
{
	info->number = 0;
	info->count = 0;

	sequence_init(&info->order);
	sequence_resize(&info->order, n);
	for(int i = 0; i < n; i++)
		vector_entry(&info->order, i) = -1;

	sequence_init(&info->link);
	sequence_resize(&info->link, n);
	for(int i = 0; i < n; i++)
		vector_entry(&info->link, i) = -1;

	sequence_init(&info->stack);
}

void _tarjan_destroy(TarjanInfo * info)
{
	sequence_destroy(&info->order);
	sequence_destroy(&info->link);
	sequence_destroy(&info->stack);
}

void _tarjan_display(TarjanInfo * info, int i)
{
	printf("\tvertex:%d:\torder: %d\tlink: %d\n",
		   i,
	       vector_entry(&info->order, i),
	       vector_entry(&info->link, i));
}

void _graph_tarjan_scc(Graph * graph, int i, TarjanInfo * info,
	                   IntegerSequence * components)
{
	info->number++;
	vector_entry(&info->order, i) = info->number;
	vector_entry(&info->link, i) = info->number;

	graph_mark_vertex_grey(graph, i);
	sequence_push_back(&info->stack, i);

	slist_for_each(pe, &graph_vertex_entry(graph, i)->firstedge) {
		Edge * edge = graph_edge_entry_of_position(pe);
		int j = edge->neighbor;

		if(graph_vertex_is_unmarked(graph, j)) {
			_graph_tarjan_scc(graph, j, info, components);
			vector_entry(&info->link, i) = _min(vector_entry(&info->link, i),
			                             vector_entry(&info->link, j));
		} else if(graph_vertex_is_grey(graph, j)) {
			vector_entry(&info->link, i) = _min(vector_entry(&info->link, i),
			                             vector_entry(&info->order, j));
		}
	}

	if(vector_entry(&info->link, i) == vector_entry(&info->order, i)) {
		int j;
		info->count++;
		do {
			sequence_get_back(&info->stack, &j);
			sequence_push_back(components, j);
			sequence_pop_back(&info->stack);
			graph_mark_vertex_black(graph, j);
		} while( i != j);
		sequence_push_back(components, -1); /* append separator */
	}
}

int graph_tarjan_scc(Graph * graph, IntegerSequence * s)
{
	int n = graph_vertex_size(graph);

	TarjanInfo info;
	_tarjan_init(&info, n);

	graph_reset_vertices(graph);
	for(int i = 0; i < n; i++)
		if(graph_vertex_is_unmarked(graph, i))
			_graph_tarjan_scc(graph, i, &info, s);

	int count = info.count;
	_tarjan_destroy(&info);
	return count;
}

/*******************************************************************************
 * Prim's algorithm for minimum spanning tree (similar to Dijkstra'a algorithm)
 ******************************************************************************/
/**
 * graph_min_dist -- find the minimum distance bewteen a vertex and the others
 * Returns 1 on modification, otherwise, returns 0
 */
static inline int
graph_min_dist(EdgeListGraph * path, int start, int end, double weight)
{
	double de = egraph_edge_entry(path, end).weight;
	if (de > weight) {
		egraph_edge_entry(path, end).start = start;
		egraph_edge_entry(path, end).weight = weight;
		return 1;
	}
	return 0;
}

/**
 * By modifying breadth-first search algorithm, for each pass, we find
 * the vertex of minimun distance and use it as the start vertex
 * of next pass.
 */
void graph_prim_linear_search(Graph * graph, EdgeListGraph * path)
{
	int n = graph_vertex_size(graph);
	graph_reset_vertices(graph);

	/* initialize path */
	path_resize(path, -1, n);

	printf("initial path: \n");
	egraph_write(path, stdout);

	int start = 0;

	for (int i = 0; i < n - 1; i++) {
		Vertex *vstart = graph_vertex_entry(graph, start);
		/* update distances */
		slist_for_each(pe, &vstart->firstedge) {
			Edge *edge = graph_edge_entry_of_position(pe);
			Vertex *u = graph_vertex_entry(graph, edge->neighbor);
			if (graph_vertex_is_unmarked(graph, edge->neighbor))
				graph_min_dist(path, start, edge->neighbor,
					       edge->weight);
		}

		/* finish visiting */
		vstart->vertstate = 1;
		printf("after visiting vertex: %d\n", start);
		egraph_write(path, stdout);

		/* find minimum distance unvisited vertex by linear searching */
		double distance = HUGE_VAL;
		start = -1;
		for (int j = 0; j < n; j++) {
			if (graph_vertex_is_unmarked(graph, j)
			    && egraph_edge_entry(path, j).weight < distance) {
				distance = egraph_edge_entry(path, j).weight;
				start = j;
			}
		}
		if (start == -1)
			break;
	}
}

void graph_prim_priority_queue(Graph * graph, EdgeListGraph * path)
{
	int n = graph_vertex_size(graph);
	graph_reset_vertices(graph);

	/* initialize path */
	path_resize(path, -1, n);

	printf("initial path:\n");
	egraph_write(path, stdout);

	/* initialize heap */
	Heap heap;
	heap_init(&heap, &path->edges, weight_compare);
	printf("initial heap:\n");
	sequence_write(&heap.h2k, stdout, " ");

	int start;

	for (int i = 0; i < n - 1; i++) {
		start = heap_pop(&heap);
		printf("visiting %d:\n", start);
		Vertex *vstart = graph_vertex_entry(graph, start);
		/* update distances */
		slist_for_each(pe, &vstart->firstedge) {
			Edge *edge = graph_edge_entry_of_position(pe);
			if (graph_vertex_is_marked(graph, edge->neighbor))
				continue;
			if (graph_min_dist
			    (path, start, edge->neighbor, edge->weight)) {
				printf("update:%d\n", edge->neighbor);
				heap_update(&heap, edge->neighbor);
				egraph_write(path, stdout);
				sequence_write(&heap.h2k, stdout, " ");
			}
		}

		/* finish visiting */
		vstart->vertstate = 1;
		printf("after visiting vertex: %d\n", start);
		egraph_write(path, stdout);
		/*heap_pop(&heap); */

		/* find minimum distance unvisited vertex */
		if (heap_isempty(&heap))
			break;
	}

	heap_destroy(&heap);
}

void graph_prim(Graph * graph, EdgeListGraph * path)
{
	return graph_prim_priority_queue(graph, path);
}

/*******************************************************************************
 * Dijkstra's algorithm for single-source shortest path
 ******************************************************************************/
/* Returns 1 on modification, otherwise, returns 0 */
static inline int
graph_relax(EdgeListGraph * path, int start, int end, double weight)
{
	double ds = egraph_edge_entry(path, start).weight;
	double de = egraph_edge_entry(path, end).weight;
	if (de > ds + weight) {
		egraph_edge_entry(path, end).start = start;
		egraph_edge_entry(path, end).weight = ds + weight;
		return 1;
	}
	return 0;
}

/* Implementation of Dijkstra's algorithm with linear search */
void
graph_dijkstra_linear_search(Graph * graph, int source, EdgeListGraph * path)
{
	int n = graph_vertex_size(graph);
	graph_reset_vertices(graph);

	/* initialize path */
	path_resize(path, source, n);

	printf("initial path start from vertex: %d\n", source);
	egraph_write(path, stdout);

	int start = source;

	for (int i = 0; i < n - 1; i++) {
		Vertex *vstart = graph_vertex_entry(graph, start);
		/* update distances */
		slist_for_each(pe, &vstart->firstedge) {
			Edge *edge = graph_edge_entry_of_position(pe);
			graph_relax(path, start, edge->neighbor, edge->weight);
		}

		/* finish visiting */
		vstart->vertstate = 1;
		printf("after visiting vertex: %d\n", start);
		egraph_write(path, stdout);

		/* find minimum distance unvisited vertex by linear searching */
		double distance = HUGE_VAL;
		start = -1;
		for (int j = 0; j < n; j++) {
			if (graph_vertex_is_unmarked(graph, j) &&
			    egraph_edge_entry(path, j).weight < distance) {
				distance = egraph_edge_entry(path, j).weight;
				start = j;
			}
		}
		if (start == -1)
			break;
	}
}

/* Implementation of Dijkstra's algorithm with indexed priority queue */
void
graph_dijkstra_priority_queue(Graph * graph, int source, EdgeListGraph * path)
{
	int n = graph_vertex_size(graph);
	graph_reset_vertices(graph);

	/* initialize path */
	path_resize(path, source, n);

	printf("initial path start from vertex: %d\n", source);
	egraph_write(path, stdout);

	/* initialize heap */
	Heap heap;
	heap_init(&heap, &path->edges, weight_compare);

	int start;

	for (int i = 0; i < n - 1; i++) {
		start = heap_pop(&heap);
		Vertex *vstart = graph_vertex_entry(graph, start);
		/* update distances */
		slist_for_each(pe, &vstart->firstedge) {
			Edge *edge = graph_edge_entry_of_position(pe);
			if (graph_relax
			    (path, start, edge->neighbor, edge->weight)) {
				heap_update(&heap, edge->neighbor);
			}
		}

		/* finish visiting */
		printf("after visiting vertex: %d\n", start);
		egraph_write(path, stdout);

		/* find minimum distance unvisited vertex */
		if (heap_isempty(&heap))
			break;
	}

	heap_destroy(&heap);
}

void graph_dijkstra(Graph * graph, int source, EdgeListGraph * path)
{
	graph_dijkstra_priority_queue(graph, source, path);
}

/*******************************************************************************
 * Bellman-Ford's algorithm for single-source shortest path
 ******************************************************************************/
/**
 * graph_bellman_ford - implement of Bellman-Ford's algorithm,
 * only processing vertices whose distances changed in the previous pass
 * @graph: graph to be process, already initialized
 * @source: the source vertex index
 * @path: paths, already initialized
 *
 * Returns 1 if there is no negative weighted cycle, otherwise, returns 0.
 *
 * NOTE: The only edges that could lead to a change in distances are those
 * leaving a vertex whose distances value changed in the previous pass.
 */
int graph_bellman_ford(Graph * graph, int source, EdgeListGraph * path)
{
	int n = graph_vertex_size(graph);
	graph_reset_vertices(graph);

	/* initialize path */
	path_resize(path, source, n);
	graph_mark_vertex(graph, source);

	IntegerSequence queue;
	sequence_init(&queue);
	sequence_push_back(&queue, source);
	sequence_push_back(&queue, -1);	/* flag -1 used to count pass */

	/**
	 * Bellman-Ford algorithm requires only n-1 pass relaxations.
	 * Here we implement n pass, if there is no negative cycle the vertices
	 * queue must only remain the separator flag -1,
	 * otherwise, the queue includes those vertices whose distances changed
	 * at the last pass.
	 */
	int pass = 0;
	while (queue.size != 0 && pass < n) {
		egraph_write(path, stdout);
		printf("vertices queue: ");
		sequence_write(&queue, stdout, " ");
		int start;
		sequence_get_front(&queue, &start);
		sequence_pop_front(&queue);
		if (start == -1) {
			pass++;
			printf("%d-th pass finished!\n\n", pass);
			sequence_push_back(&queue, -1);
			continue;
		}

		/* reset the vertex state */
		graph_unmark_vertex(graph, start);

		slist_for_each(pe, &graph_vertex_entry(graph, start)->firstedge) {
			Edge *edge = graph_edge_entry_of_position(pe);
			if (graph_relax(path, start, edge->neighbor, edge->weight)) {
				/* mark it so as to be processed in next pass */
				if (graph_vertex_is_unmarked(graph, edge->neighbor)) {
					graph_mark_vertex(graph, edge->neighbor);
					sequence_push_back(&queue, edge->neighbor);
				}
			}
		}
	}

	/* detect negative cycle */
	int queuesize = queue.size;
	sequence_destroy(&queue);
	/* only the last flag -1 is allowed in queue */
	if (queuesize != 1)
		return 0;
	return 1;
}

void graph_create(EdgeListGraph * src, Graph * dest)
{
	int n = egraph_vertex_size(src);
	for(int i = 0; i < n; i++)
		graph_add_vertex(dest);

	printf("%d\n", n);

	graph_write(dest, stdout);

	dest->isdirected = src->isdirected;

	for(int i = 0; i < n; i++) {
		if(egraph_edge_entry(src, i).start != -1) {
			graph_add_edge(dest,
			               egraph_edge_entry(src, i).start,
			               egraph_edge_entry(src, i).end,
			               egraph_edge_entry(src, i).weight);
		}
	}
}
