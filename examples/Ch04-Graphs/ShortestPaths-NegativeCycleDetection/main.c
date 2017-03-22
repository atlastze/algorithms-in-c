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

int main(int argc, char *argv[])
{
	if (argc < 2)
		return 0;

	Graph graph;
	graph_init(&graph, 1);

	FILE *fp = fopen(argv[1], "r");
	if (!fp)
		exit(1);

	graph_read(&graph, fp);
	fclose(fp);

	graph_write(&graph, stdout);

	EdgeListGraph path;

	egraph_init(&path, 1);

	if (graph_bellman_ford(&graph, 0, &path))
		printf("There is no negative weighted cycle!\n");
	else {
		Graph subgraph;
		graph_init(&subgraph, 1);
		egraph_write(&path, stdout);
		graph_create(&path, &subgraph);
		graph_write(&subgraph, stdout);

		IntegerSequence s;
		sequence_init(&s);
		graph_tarjan_scc(&subgraph, &s);
		printf("the strongly connected components are:\n");
		sequence_write(&s, stdout, " ");

		sequence_destroy(&s);
		graph_destroy(&subgraph);
	}

	egraph_destroy(&path);

	/* destroy graph */
	graph_destroy(&graph);

	return 0;
}
