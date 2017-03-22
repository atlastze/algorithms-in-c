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
#include <string.h>
#include "IntrusiveLinkedList.h"

typedef struct _Student {
	char name[32];
	int age;
	IntrusiveDListNode link;
} Student;

void show_student(IntrusiveDList * head)
{
	dlist_for_each(position, head) {
		Student *student = dlist_entry(position->next, Student, link);
		printf("student address: %p, name: %s, age: %d\n",
		       student, student->name, student->age);
	}
}

/**
 * find_student -- find student in a list
 * @head: student list head
 * @name: student name to find in a list
 *
 * Returns list node position on success, or NULL on failure.
 * NOTE: POSITION is defined as pointer to prevoious list node.
 */
IntrusiveDListNode *find_student(IntrusiveDList * head, char *name)
{
	dlist_for_each(position, head) {
		Student *student = dlist_entry(position->next, Student, link);
		if (strcmp(student->name, name) == 0)
			return position;
	}
	return NULL;
}

void use_intrusive_dlist()
{
	printf("\nTesting intrusive doubly linked list...\n");
	IntrusiveDList head;
	dlist_init(&head);

	char *names[] = { "Tonny", "Andy", "Michael", "Leslie", "John" };
	int ages[] = { 12, 10, 11, 14, 13 };
	for (int i = 0; i < 5; i++) {
		Student *student = (Student *) malloc(sizeof(Student));
		strcpy(student->name, names[i]);
		student->age = ages[i];
		dlist_init(&(student->link));
		dlist_insert_back(&head, &(student->link));
	}
	printf("Student list after populating:\n");
	show_student(&head);

	IntrusiveDListNode *position = find_student(&head, "Michael");
	if (position) {
		Student *student = dlist_entry(position->next, Student, link);
		dlist_remove_back(position);
		free(student);
		printf("\nStudent list after deletion:\n");
		show_student(&head);
	}

	dlist_destroy(&head, Student, link, free);
}

typedef struct _Book {
	char title[32];
	double price;
	IntrusiveSListNode link;
} Book;

void show_book(IntrusiveSList * head)
{
	slist_for_each(position, head) {
		Book *book = slist_entry(position->next, Book, link);
		printf("book address: %p, title: %s, price: %8.2f\n",
		       book, book->title, book->price);
	}
}

/**
 * find_book -- find book in a list
 * @head: book list head
 * @title: book title to find in a list
 *
 * Returns list node position on success, or NULL on failure.
 * NOTE: POSITION is defined as pointer to prevoious list node.
 */
IntrusiveSListNode *find_book(IntrusiveSList * head, char *title)
{
	slist_for_each(position, head) {
		Book *book = slist_entry(position->next, Book, link);
		if (strcmp(book->title, title) == 0)
			return position;
	}
	return NULL;
}

void use_intrusive_slist()
{
	printf("\nTesting intrusive singly linked list...\n");
	IntrusiveSList head;
	slist_init(&head);

	char *titles[] = { "C Programming", "C++ Programming", "Internet",
		"Compiler Design", "Data Structures"
	};
	double prices[] = { 12.8, 10.5, 11.9, 14.5, 13.99 };
	for (int i = 0; i < 5; i++) {
		Book *book = (Book *) malloc(sizeof(Book));
		strcpy(book->title, titles[i]);
		book->price = prices[i];
		slist_init(&(book->link));
		slist_insert_back(&head, &(book->link));
	}
	printf("Book list after populating:\n");
	show_book(&head);

	IntrusiveSListNode *position = find_book(&head, "Internet");
	if (position) {
		Book *book = slist_entry_of_position(position, Book, link);
		slist_remove_back(position);
		free(book);
		printf("\nBook list after deletion:\n");
		show_book(&head);
	}

	slist_destroy(&head, Book, link, free);
}

int main(int argc, char *argv[])
{
	use_intrusive_dlist();
	use_intrusive_slist();
	return 0;
}
