%{
#include <stdlib.h>
#include "SymbolTable.h"

int chars = 0;
int words = 0;
int lines = 0;

SymbolTable symtab;
int columns = 0;
%}

%%
[a-zA-z]+ {
	if(!symtab_find(&symtab, yytext)) {
		/* allocate symbol node dynamically, we destroy them at the end of main */
		Symbol * symbol = (Symbol *)malloc(sizeof(Symbol));
		strncpy(symbol->name, yytext, 63);
		symbol->line = lines+1;
		symbol->column = columns;
		symtab_enter(&symtab, symbol);
	}

	words++;
	chars += yyleng;
	columns += yyleng;
}
\n        { chars++; lines++; columns = 0;   }
.         { chars++; columns++;              }
%%

int yywrap(){
   return 1;
}
