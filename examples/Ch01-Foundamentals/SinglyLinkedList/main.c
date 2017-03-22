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

#include "SinglyLinkedList.h"
#include <stdio.h>

COMPARE_IMPL(intcmp, int)

int main(int argc, char *argv[])
{
	int a[] = { 1, 2, 3, 4, 5 };
	SinglyLinkedList head;
	slist_init(&head);
	slist_insert_back(&head, &a[0]);
	slist_insert_back(&head, &a[1]);
	slist_insert_back(&head, &a[2]);
	slist_insert_back(&head, &a[3]);
	slist_insert_back(&head, &a[4]);
	slist_for_each(position, &head) {
		printf("node: %p, element: %d\n", position->next,
		       *((int *)(position->next->element)));
	}

	int index = 4;
	SinglyLinkedListNode *position = slist_find(&head, &a[index], intcmp);
	printf("find %d at node %p\n", a[index], position->next);

	slist_destroy(&head, NULL);

	return 0;
}
