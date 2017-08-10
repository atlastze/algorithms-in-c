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

/***********************************************************
 Syntax of graph file (Subset of dot language):
    graph_definition ::= digraph | graph
    digraph ::= 'digraph' '{' (digraph_edge) * '}'
    graph ::= 'graph' '{' (graph_edge) * '}'
    digraph_edge ::= integer '->' integer (weight_attribute)?
    graph_edge ::= integer '--' integer (weight_attribute)?
    weight_attribute ::= '[' 'label' '=' '"' number '"' ']'
    number ::= ( '+' | '-' )? ( integer ( '.' integer? )? | '.' integer ) ( ( 'e' | 'E' ) ( '+' | '-' )? integer )?
    integer ::= ( '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9' )+
 **********************************************************/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "EdgeListGraph.h"

/***********************************************************
 * CharStream -- characters stream
 **********************************************************/
struct CharStream {
	FILE *fp;		/* source file */
	int row;		/* current line number */
	int column;		/* current line position, start from 1 */
	char ch;		/* current character */
};

/* Initialize characters stream */
void charstream_init(struct CharStream *charStream, FILE * fp)
{
	charStream->fp = fp;
	charStream->row = 1;
	charStream->column = 1;
	charStream->ch = fgetc(charStream->fp);
}

/* Return the source character at the current position. */
char charstream_current_char(struct CharStream *charStream)
{
	return charStream->ch;
}

/* Consume the current source character and return the next character. */
char charstream_next_char(struct CharStream *charStream)
{
	charStream->ch = fgetc(charStream->fp);
	charStream->column++;
	return charStream->ch;
}

/***********************************************************
 * Scanner -- generating tokens stream
 **********************************************************/
/* Token types */
int None = 256, Integer = 257, Float = 258, Plus = 259, Minus = 260;
/* Key word types */
int K_Digraph = 271, K_Graph = 272, K_Label = 273;
/* Operators */
int K_Arrow = 281, K_Line = 282;

struct Token {
	char text[64];		/* token string */
	int row;		/* line number */
	int column;		/* column number */
	int type;		/* token type */
};

struct Scanner {
	struct CharStream *charStream;	/* source code */
	struct Token token;	/* current token */
};

/* Call struct CharStream's method */
char scanner_current_char(struct Scanner *scanner)
{
	return charstream_current_char(scanner->charStream);
}

/* Call struct CharStream's method */
char scanner_next_char(struct Scanner *scanner)
{
	return charstream_next_char(scanner->charStream);
}

/* Returns current token */
struct Token scanner_current_token(struct Scanner *scanner)
{
	return scanner->token;
}

/* Check whether a character is white space */
int isWhiteSpace(char ch)
{
	return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r';
}

void scanner_skip_whitespace(struct Scanner *scanner)
{
	while (isWhiteSpace(scanner_current_char(scanner))) {
		if (scanner_current_char(scanner) == '\n') {
			scanner->charStream->row++;
			scanner->charStream->column = 0;
		}
		scanner_next_char(scanner);
	}
}

/* Reset current token */
void scanner_init_token(struct Scanner *scanner)
{
	memset(scanner->token.text, 0, 64);
	scanner->token.row = scanner->charStream->row;
	scanner->token.column = scanner->charStream->column;
	scanner->token.type = None;
}

/* Append current character to current token's text */
void scanner_enter_char(struct Scanner *scanner)
{
	int n = strlen(scanner->token.text);
	if (n >= 63)
		return;
	scanner->token.text[n] = scanner->charStream->ch;
}

/* Set current token text */
void scanner_set_text(struct Scanner *scanner, char *text)
{
	strncpy(scanner->token.text, text, 64);
}

/* Set current token type */
void scanner_set_type(struct Scanner *scanner, int type)
{
	scanner->token.type = type;
}

/* Scanning keyword
   Return 1 on success, otherwise, return 0 */
int scanner_keyword(struct Scanner *scanner)
{
	if (!isalpha(scanner_current_char(scanner)))
		return 0;

	while (isalpha(scanner_current_char(scanner))) {
		scanner_enter_char(scanner);
		scanner_next_char(scanner);
	}

	if (strcmp(scanner->token.text, "digraph") == 0)
		scanner_set_type(scanner, K_Digraph);
	else if (strcmp(scanner->token.text, "graph") == 0)
		scanner_set_type(scanner, K_Graph);
	else if (strcmp(scanner->token.text, "label") == 0)
		scanner_set_type(scanner, K_Label);
	else {			/* error */
		scanner_set_type(scanner, None);
		return 0;
	}
	return 1;
}

/* Scanning unsigned integer
   Return 1 on success, otherwise, return 0 */
int scanner_integer_literal(struct Scanner *scanner)
{
	if (!isdigit(scanner_current_char(scanner)))
		return 0;

	while (isdigit(scanner_current_char(scanner))) {
		scanner_enter_char(scanner);
		scanner_next_char(scanner);
	}
	return 1;
}

/* Scanning unsigned number:
   number ::= ( integer ( '.' integer? )? | '.' integer ) ( ( 'e' | 'E' ) ( '+' | '-' )? integer )?
   Return 1 on success, otherwise, return 0
 */
int scanner_numeric_literal(struct Scanner *scanner)
{
	scanner_set_type(scanner, Integer);	/* default type */

	if (isdigit(scanner_current_char(scanner))) {
		scanner_integer_literal(scanner);
		if (scanner_current_char(scanner) == '.') {
			scanner_set_type(scanner, Float);
			scanner_enter_char(scanner);
			scanner_next_char(scanner);
			if (isdigit(scanner_current_char(scanner))) {
				scanner_integer_literal(scanner);
			}
		}
	} else if (scanner_current_char(scanner) == '.') {
		scanner_set_type(scanner, Float);
		scanner_enter_char(scanner);
		scanner_next_char(scanner);
		if (!scanner_integer_literal(scanner))
			return 0;
	} else
		return 0;

	if (scanner_current_char(scanner) == 'e'
	    || scanner_current_char(scanner) == 'E') {
		scanner_set_type(scanner, Float);
		scanner_enter_char(scanner);
		scanner_next_char(scanner);
		if (scanner_current_char(scanner) == '+' ||
		    scanner_current_char(scanner) == '-') {
			scanner_enter_char(scanner);
			scanner_next_char(scanner);
		}
		if (!scanner_integer_literal(scanner))
			return 0;
	}

	return 1;
}

/* Consume the current token and return the next token . */
struct Token scanner_next_token(struct Scanner *scanner)
{
	scanner_skip_whitespace(scanner);
	scanner_init_token(scanner);

	char ch = scanner_current_char(scanner);

	if (isdigit(ch) || ch == '.') {
		scanner_numeric_literal(scanner);
	} else if (isalpha(ch)) {
		scanner_keyword(scanner);
	} else if (ch == '+') {
		scanner_next_char(scanner); /* skip and advance */
		scanner_numeric_literal(scanner);
	} else if (ch == '-') {
		scanner_enter_char(scanner);
		char lookaheadChar = scanner_next_char(scanner);
		if (lookaheadChar == '>') {
			scanner_set_type(scanner, K_Arrow);
			scanner_enter_char(scanner);
			scanner_next_char(scanner);
		} else if (lookaheadChar == '-') {
			scanner_set_type(scanner, K_Line);
			scanner_enter_char(scanner);
			scanner_next_char(scanner);
		} else if (isdigit(lookaheadChar)) {
			scanner_numeric_literal(scanner);
		} else {	/* error */

		}
	} else if (ch == EOF) {
		scanner_set_text(scanner, "EOF");
		scanner_set_type(scanner, EOF);
	} else {
		scanner_set_type(scanner, ch);
		scanner_enter_char(scanner);
		scanner_next_char(scanner);
	}

#if 0
	printf(".. Scanning token: %s, position: (%d, %d), type: %d\n",
	       scanner->token.text,
	       scanner->token.row, scanner->token.column, scanner->token.type);
#endif

	return scanner->token;
}

/* Initialize a scanner */
void scanner_init(struct Scanner *scanner, struct CharStream *charStream)
{
	scanner->charStream = charStream;
	scanner_next_token(scanner);
}

/***********************************************************
 * State
 **********************************************************/
struct State {
	EdgeListGraph *egraph;	/* to which we write data */
	int start;		/* current start vertex index  */
	int end;		/* currentend vertex index    */
	double weight;		/* weight of the current edge  */
};

void state_init_current_edge(struct State *state)
{
	state->start = -1;
	state->end = -1;
	state->weight = -1.0;
}

void state_init(struct State *state, EdgeListGraph * egraph)
{
	state->egraph = egraph;
	state_init_current_edge(state);
}

void state_set_graph(struct State *state, int isdirected)
{
	state->egraph->isdirected = isdirected;
}

void state_set_vertex(struct State *state, int index)
{
	if (state->start == -1)
		state->start = index;
	else if (state->end == -1)
		state->end = index;

	state->egraph->vcount = state->egraph->vcount > index + 1 ?
	    state->egraph->vcount : index + 1;
}

void state_set_weight(struct State *state, double weight)
{
	state->weight = weight;
}

void state_enter_edge(struct State *state)
{
	EdgeNode edge = { state->start, state->end, state->weight };
	edgelist_push_back(&state->egraph->edges, edge);
}

/***********************************************************
 * Parser
 **********************************************************/
/* Actions */
int ACTION_NONE = 0;		/* do nothing */
int ACTION_VERTEX = 1;		/* set vertex index */
int ACTION_WEIGHT = 2;		/* set edge weight */

struct Parser {
	struct Scanner *scanner;	/* from where we get tokens */
	struct State *state;
	int action;
};

/* Initialize a parser */
void parser_init(struct Parser *parser, struct Scanner *scanner,
		 struct State *state)
{
	parser->scanner = scanner;
	parser->state = state;
	parser->action = ACTION_NONE;
}

/* Call struct Scanner's method */
struct Token parser_current_token(struct Parser *parser)
{
	return scanner_current_token(parser->scanner);
}

/* Call struct Scanner's method */
struct Token parser_next_token(struct Parser *parser)
{
	return scanner_next_token(parser->scanner);
}

/* Check whether the current token matches the specific type */
int parser_match(struct Parser *parser, int type)
{
	if (parser->scanner->token.type != type)
		return 0;

	/* actions */
	if (type == Integer && parser->action == ACTION_VERTEX) {
		int index = atoi(parser->scanner->token.text);
		//printf("index: %d\n", index);
		state_set_vertex(parser->state, index);
	} else if ((type == Integer || type == Float)
		   && parser->action == ACTION_WEIGHT) {
		double weight = atof(parser->scanner->token.text);
		//printf("weight: %f\n", weight);
		state_set_weight(parser->state, weight);
	}

	if (type != EOF)
		parser_next_token(parser);
	return 1;
}

/* Parsing number */
int parser_number(struct Parser *parser)
{
	if (parser_match(parser, Integer) || parser_match(parser, Float))
		return 1;
	else
		return 0;
}

/* Parsing weight attribute */
int parser_weight_attribute(struct Parser *parser)
{
	if (!parser_match(parser, '[') ||
	    !parser_match(parser, K_Label) ||
	    !parser_match(parser, '=') || !parser_match(parser, '"'))
		return 0;

	if (!parser_number(parser))
		return 0;

	if (!parser_match(parser, '"') || !parser_match(parser, ']'))
		return 0;

	return 1;
}

/* Parsing graph edge */
int parser_graph_edge(struct Parser *parser)
{
	state_init_current_edge(parser->state);
	parser->action = ACTION_VERTEX;
	if (!parser_match(parser, Integer) ||
	    !parser_match(parser, K_Line) || !parser_match(parser, Integer))
		return 0;

	if (parser_current_token(parser).type == '[') {
		parser->action = ACTION_WEIGHT;
		if (!parser_weight_attribute(parser))
			return 0;
	}

	state_enter_edge(parser->state);

	return 1;
}

/* Parsing graph */
int parser_graph(struct Parser *parser)
{
	if (!parser_match(parser, K_Graph) || !parser_match(parser, '{'))
		return 0;

	while (parser_current_token(parser).type != '}') {
		if (!parser_graph_edge(parser))
			return 0;
	}

	return parser_match(parser, '}')
	    && parser_match(parser, EOF);
}

/* Parsing digraph edge */
int parser_digraph_edge(struct Parser *parser)
{
	state_init_current_edge(parser->state);
	parser->action = ACTION_VERTEX;
	if (!parser_match(parser, Integer) ||
	    !parser_match(parser, K_Arrow) || !parser_match(parser, Integer))
		return 0;

	if (parser_current_token(parser).type == '[') {
		parser->action = ACTION_WEIGHT;
		if (!parser_weight_attribute(parser))
			return 0;
	}

	state_enter_edge(parser->state);

	return 1;
}

/* Parsing digraph */
int parser_digraph(struct Parser *parser)
{
	if (!parser_match(parser, K_Digraph)
	    || !parser_match(parser, '{'))
		return 0;

	while (parser_current_token(parser).type != '}') {
		if (!parser_digraph_edge(parser))
			return 0;
	}

	return parser_match(parser, '}')
	    && parser_match(parser, EOF);
}

/* Parsing graph definition */
int parser_graph_definition(struct Parser *parser)
{
	if (parser_current_token(parser).type == K_Digraph) {
		state_set_graph(parser->state, 1);
		return parser_digraph(parser);
	} else if (parser_current_token(parser).type == K_Graph) {
		state_set_graph(parser->state, 0);
		return parser_graph(parser);
	} else {
		return 0;
	}
}

/* Save a graph to a dot file */
void graph2dot(FILE * fp, struct _EdgeListGraph *egraph)
{
	char name[32] = "graph {", edgeop[4] = "--";
	if (egraph->isdirected) {
		strcpy(name, "digraph {");
		strcpy(edgeop, "->");
	}
	fprintf(fp, "%s\n", name);
	for (int i = 0; i < egraph->edges.size; i++) {
		EdgeNode edge;
		edgelist_get(&egraph->edges, i, &edge);
		fprintf(fp, "    %d %s %d [ label = \"%0.2lf\" ]\n",
			edge.start, edgeop, edge.end, edge.weight);
	}
	fprintf(fp, "}\n");
}

/* Read a graph from a dot file */
int dot2graph(FILE * fp, struct _EdgeListGraph *egraph)
{
	struct CharStream charStream;
	charstream_init(&charStream, fp);
	struct Scanner scanner;
	scanner_init(&scanner, &charStream);

	struct State state;
	state_init(&state, egraph);

	struct Parser parser;
	parser_init(&parser, &scanner, &state);

	if (!parser_graph_definition(&parser)) {
		printf("Syntax error!\n");
		return 0;
	}
	return 1;
}

#if 0
/* Test */
int main(int argc, char *argv[])
{
	if (argc < 2)
		return 0;

	FILE *fp = fopen(argv[1], "r");

	struct CharStream charStream;
	charstream_init(&charStream, fp);
	struct Scanner scanner;
	scanner_init(&scanner, &charStream);
	struct Parser parser;
	parser_init(&parser, &scanner);
	if (parser_graph_definition(&parser))
		printf("Accepted!\n");
	else
		printf("Syntax error!\n");

	fclose(fp);

	return 0;
}
#endif
