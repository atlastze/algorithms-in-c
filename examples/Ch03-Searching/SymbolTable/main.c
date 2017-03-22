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

#include "SymbolTable.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	/* declare and initialize symbol table */
	SymbolTable symtab;
	symtab_init(&symtab, BKDRHash);

	/* symbol node is static, so we need not deleted them at the end of
	 * the function. Alternatively, we can allocate symbol dynamically and
	 * managed memeory allocated manually.
	 */
	Symbol var1 = { "age", 1, 2 };
	Symbol var2 = { "birthday", 3, 4 };
	Symbol var3 = { "school", 5, 6 };

	/* enter symbols */
	symtab_enter(&symtab, &var1);
	symtab_enter(&symtab, &var2);
	symtab_enter(&symtab, &var3);

	/* retrieve symbol by name */
	Symbol *symbol = symtab_retrieve(&symtab, "birthday");
	if (symbol) {
		printf("Symbol Info\n\tname: %s\n\tline: %d\n\tcolumn: %d\n",
		       symbol->name, symbol->line, symbol->column);
	} else {
		printf("Symbol not found!\n");
	}

	return 0;
}
