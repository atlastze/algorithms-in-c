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

void symtab_init(SymbolTable * symtab, HashCode hashCode)
{
	for (int i = 0; i < BUCKET_SIZE; i++)
		slist_init(&symtab->list[i]);
	symtab->hashCode = hashCode;
}

int symtab_enter(SymbolTable * symtab, Symbol * symbol)
{
	int i = symtab_hash(symtab, symbol->name);
	slist_insert_back(&symtab->list[i], &symbol->link);
	return i;
}

IntrusiveSListNode *symtab_find(SymbolTable * symtab, char *name)
{
	int i = symtab_hash(symtab, name);
	slist_for_each(position, &(symtab->list[i])) {
		Symbol *symbol =
		    slist_entry_of_position(position, Symbol, link);
		if (strcmp(symbol->name, name) == 0)
			return position;
	}
	return NULL;
}

Symbol *symtab_retrieve(SymbolTable * symtab, char *name)
{
	IntrusiveSListNode *position;
	position = symtab_find(symtab, name);
	if (position)
		return slist_entry_of_position(position, Symbol, link);
	return NULL;
}
