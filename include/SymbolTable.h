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

#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "IntrusiveLinkedList.h"
#include "HashCode.h"
#include <string.h>

#define BUCKET_SIZE 97

typedef struct _Symbol {
    char name[64];
    int line;
    int column;
    IntrusiveSListNode link;
} Symbol;

typedef struct _SymbolTable {
    IntrusiveSList list[BUCKET_SIZE];
    HashCode hashCode;          /* hash code function pointer */
} SymbolTable;

/**
 * symtab_init - initialize a symbol table
 * @symtab: symbol table to be initialized.
 * @hashCode: hash code function pointer
 */
void symtab_init(SymbolTable * symtab, HashCode hashCode);

/**
 * symtab_enter - enter a symbol
 * @symtab: symbol table to be updated.
 * @symbol: symbol to be entered.
 *
 * Returns hash code (bucket index)
 *
 * NOTE: This function will check wheterh the symbol is already existed. Thus,
 * you should check that before entering a symbol. The newly entered symbol is
 * always inserted after the dummy head.
 */
int symtab_enter(SymbolTable * symtab, Symbol * symbol);

/**
 * symtab_find - retrieve a symbol's position (POSITION is defined as pointer
 * to prevoious node)
 * @symtab: symbol table to be searched in.
 * @name: symbol name to be retrieved.
 *
 * Returns position on success, NULL if symbol not existed
 */
IntrusiveSListNode *symtab_find(SymbolTable * symtab, char *name);

/**
 * symtab_retrieve - retrieve a symbol
 * @symtab: symbol table to be searched in.
 * @name: symbol name to be retrieved.
 *
 * Returns symbol pointer on success, NULL on symbol not existed
 */
Symbol *symtab_retrieve(SymbolTable * symtab, char *name);

/**
 * symtab_hash - return index of a key
 * @symtab: symbol table to be searched in.
 * @name: symbol name to be retrieved.
 *
 * Returns index of a key
 */
static inline int symtab_hash(SymbolTable * symtab, char *name)
{
    return symtab->hashCode(name, strlen(name)) % BUCKET_SIZE;
}

#endif /* SYMBOLTABLE_H */
