#include <stdio.h>
#include <stdlib.h>
#include "SymbolTable.h"

extern int chars;
extern int words;
extern int lines;

extern SymbolTable symtab;
extern int yylex(void);

int main(int argc, char **argv)
{
	symtab_init(&symtab, RSHash);
	yylex();
	printf("Basic information:\n\tchars:%d\n\twords:%d\n\tlines:%d\n\n",
	       chars, words, lines);

	printf("Hash frequencies of distinct words in symbol table:\n");
	size_t total = 0;
	for (int i = 0; i < BUCKET_SIZE; i++) {
		size_t count = slist_size(&symtab.list[i]);
		total += count;
		printf("BUCKET NO.%3d: %zu\n", i, count);
	}
	printf("\nTotal distinct words: %zu\n", total);

	/* retrieve word */
	char name[] = "Charles";
	Symbol *symbol = symtab_retrieve(&symtab, name);
	if (symbol) {
		printf("\nThe word \'%s\' is at (%d, %d).\n",
		       symbol->name, symbol->line, symbol->column);
	} else {
		printf("\nThe word \'%s\' not found!\n", name);
	}

	/* destroy symbol table */
	for (int i = 0; i < BUCKET_SIZE; i++) {
		slist_destroy(&symtab.list[i], Symbol, link, free);
	}

	exit(0);
}
