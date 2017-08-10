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

#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <stdio.h>

/**
 * Implementation of Binary Search Tree
 * see Mark Allen Wesiss, Data Structures and Algorithm Analysis in C, 2e
 */

typedef int BSTelementtype;

typedef struct _BinarySearchTree {
	BSTelementtype element;
	struct _BinarySearchTree *lchild;
	struct _BinarySearchTree *rchild;
} BinarySearchTree;

#define bst_retrieve(position) ((position)->element)

BinarySearchTree *bst_create_node(BSTelementtype element);
void bst_destroy(BinarySearchTree * root);
BinarySearchTree *bst_search(BinarySearchTree * root, BSTelementtype element);
BinarySearchTree *bst_insert(BinarySearchTree * root, BSTelementtype element);
BinarySearchTree *bst_remove(BinarySearchTree * root, BSTelementtype element);
BinarySearchTree *bst_minimum(BinarySearchTree * root);
BinarySearchTree *bst_maximum(BinarySearchTree * root);

BinarySearchTree *bst_read(FILE * fp);

/**
 * preorder traversal
 */
void bst_write(FILE * fp, BinarySearchTree * root);

/**
 * inorder traversal
 */
void bst_inorder(BinarySearchTree * root);

#endif /* BINARY_SEARCH_TREE_H */
