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
#include "BinarySearchTree.h"

BinarySearchTree *bst_create_node(int element)
{
	BinarySearchTree *bst =
	    (BinarySearchTree *) malloc(sizeof(BinarySearchTree));
	bst->element = element;
	bst->lchild = NULL;
	bst->rchild = NULL;
	return bst;
}

void bst_destroy(BinarySearchTree * root)
{
	if (root == NULL)
		return;

	if (root->lchild)
		bst_destroy(root->lchild);
	if (root->rchild)
		bst_destroy(root->rchild);
	free(root);
}

BinarySearchTree *bst_search(BinarySearchTree * root, BSTelementtype element)
{
	if (root == NULL)
		return NULL;
	if (element < root->element)
		return bst_search(root->lchild, element);
	else if (element > root->element)
		return bst_search(root->rchild, element);
	else
		return root;
}

BinarySearchTree *bst_minimum(BinarySearchTree * root)
{
	if (root == NULL)
		return NULL;

	while (root->lchild != NULL)
		root = root->lchild;
	return root;
}

BinarySearchTree *bst_maximum(BinarySearchTree * root)
{
	if (root == NULL)
		return NULL;

	while (root->rchild != NULL)
		root = root->rchild;
	return root;
}

BinarySearchTree *bst_insert(BinarySearchTree * root, BSTelementtype element)
{
	if (root == NULL)	/* create and return a one-node tree */
		return bst_create_node(element);
	else if (element < root->element)
		root->lchild = bst_insert(root->lchild, element);
	else if (element > root->element)
		root->rchild = bst_insert(root->rchild, element);
	/* else the element is in the tree already; we'll do nothing */

	return root;		/* Do not forget this line!! */
}

BinarySearchTree *bst_remove(BinarySearchTree * root, BSTelementtype element)
{
	BinarySearchTree *tmpCell;

	if (root == NULL)
		return NULL;
	else if (element < root->element)	/* go left  */
		root->lchild = bst_remove(root->lchild, element);
	else if (element > root->element)	/* go right */
		root->rchild = bst_remove(root->rchild, element);
	/* found element to be deleted */
	else if (root->lchild && root->rchild) {	/* 2 children */
		/* replace with smallest in right subtree */
		tmpCell = bst_minimum(root->rchild);
		root->element = tmpCell->element;
		root->rchild = bst_remove(root->rchild, root->element);
	} else {		/* 1 or 0 children */
		tmpCell = root;
		if (root->lchild == NULL)	/* also handles 0 children */
			root = root->rchild;
		else if (root->rchild == NULL)
			root = root->lchild;
		free(tmpCell);
	}

	return root;
}

BinarySearchTree *bst_read(FILE * fp)
{
	BinarySearchTree *root = NULL;
	BSTelementtype element;
	while (fscanf(fp, "%d", &element) == 1)
		root = bst_insert(root, element);
	return root;
}

void bst_write(BinarySearchTree * root, FILE * fp)
{
	if (root == NULL)
		return;
	fprintf(fp, "%d ", root->element);
	bst_write(root->lchild, fp);
	bst_write(root->rchild, fp);
}

void bst_inorder(BinarySearchTree * root)
{
	if (root == NULL)
		return;
	bst_inorder(root->lchild);
	printf("%d ", root->element);
	bst_inorder(root->rchild);
}
