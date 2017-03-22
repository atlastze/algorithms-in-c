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

#include <stdio.h>
#include <stdlib.h>
#include "BinarySearchTree.h"

int main(int argc, char *argv[])
{
	BinarySearchTree *root = NULL;
	BinarySearchTree *position;

	FILE *fp = stdin;
	if (argc >= 2) {
		fp = fopen(argv[1], "r");
		if (!fp)
			exit(1);
	} else {
		return 0;
	}

	root = bst_read(fp);

	if (argc >= 2)
		fclose(fp);

	printf("preorder traversal:\n");
	bst_write(root, stdout);

	printf("\ninorder traversal:\n");
	bst_inorder(root);
	printf("\n");

	BSTelementtype element;
	printf
	    ("\nplease input the element to be deleted (non-digits to stop): ");
	while (scanf("%d", &element) == 1) {
		root = bst_remove(root, element);

		printf("\npreorder traversal:\n");
		bst_write(root, stdout);

		printf("\ninorder traversal:\n");
		bst_inorder(root);

		printf
		    ("\n\nplease input the element to be deleted (non-digits to stop): ");
	}

	bst_destroy(root);

	return 0;
}
