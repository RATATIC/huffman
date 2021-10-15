/*
* @file main.c
* @author Renat Kagal <kagal@itspartner.net>
*
* Assembling : gcc -Wall -c encode.c -o encode.o
*
* Description : encode Huffman algo
*
* Copyright (c) 2021, ITS Partner LLC.
* All rights reserved.
*
* This software is the confidential and proprietary information of
* ITS Partner LLC. ("Confidential Information"). You shall not
* disclose such Confidential Information and shall use it only in
* accordance with the terms of the license agreement you entered into
* with ITS Partner.
*/

#define _GNU_SOURCE

#include "head_encode.h"

struct List_node {
	char c;
	int count;

	struct List_node* next;
	struct List_node* right;
	struct List_node* left;
};

struct char_code {
	char* code;
	char c;
};

void encode (char* str, char* result) {
	struct List_node* list = char_counting (str);

	struct List_node* pl = list;

	while (pl != NULL) {
		printf ("%c - %d \n", pl->c, pl->count);
		pl = pl->next;
	}
	int size_char_code = 0;
	pl = list;

	while (pl != NULL) {
		size_char_code++;
		pl = pl->next;
	}
	struct List_node** cpy_list = (struct List_node**)malloc (sizeof (struct List_node*) * size_char_code);

	pl = list;
	for (int i = 0; i < size_char_code; i++) {
		cpy_list[i] = pl;
		pl = pl->next; 
	}
	struct List_node* tree = make_tree (list);
	struct char_code* str_code = creating_char_code (cpy_list, size_char_code);

	for (int i = 0; i < size_char_code; i++) {
		printf ("%s", str_code[i].code);
	}

	free_list (&list);
}

struct char_code* creating_char_code (struct List_node** cpy_list, int size_char_code) {
	struct char_code* tmp = (struct char_code*)malloc (sizeof (struct char_code) * size_char_code);
	for (int i = 0; i < size_char_code; i++) {
		tmp[i].c = cpy_list[i]->c;
		
		while (cpy_list[i] != NULL) {
			if (cpy_list[i]->next->left == cpy_list[i]) {
				asprintf (&tmp[i].code, "%s%c", tmp[i].code,'0');
			}
			else {
				asprintf (&tmp[i].code, "%s%c", tmp[i].code,'1');
			}
			cpy_list[i] = cpy_list[i]->next;
		}
	}
	return tmp;
}

struct List_node* make_tree (struct List_node* list) {
	struct List_node* tmp = NULL;
	struct List_node* first_small_node = NULL;
	struct List_node* second_small_node = NULL;
	while(list->next != NULL) {
		find_two_small_nodes (list, &first_small_node, &second_small_node);

		printf ("%c  %c\n", first_small_node->c, second_small_node->c);

		if (first_small_node != list && second_small_node != list)
			tmp = creating_union_of_two_small_nodes (list, first_small_node, second_small_node);
		else 
			if (first_small_node == list)
				tmp = change_top_and_creating_union (&list, first_small_node, second_small_node);
			else 
				tmp = change_top_and_creating_union (&list, second_small_node, first_small_node);
	}

	return tmp;
}

struct List_node* change_top_and_creating_union (struct List_node** list, struct List_node* right, struct List_node* left) {
	struct List_node* tmp = (struct List_node*)malloc (sizeof (struct List_node));

	if (tmp == NULL) {
		puts ("Failed alloc memory for tmp in creating_union_of_two_small_nodes");
		exit(EXIT_FAILURE);
	}
	tmp->c = '1';
	tmp->count = right->count + left->count;
	tmp->right = right;
	tmp->left = left;
	if (right->next->next != NULL)
		tmp->next = right->next;
	else
		tmp->next = NULL;

	struct List_node* tmp2 = (*list);
	while (tmp2->next == left) 
		tmp2 = tmp2 -> next;
	tmp2->next = left->next;

	(*list) = tmp;
}

struct List_node* creating_union_of_two_small_nodes (struct List_node* list, struct List_node* right, struct List_node* left) {
	struct List_node* tmp = (struct List_node*)malloc (sizeof (struct List_node));

	if (tmp == NULL) {
		puts ("Failed alloc memory for tmp in creating_union_of_two_small_nodes");
		exit(EXIT_FAILURE);
	}
	tmp->c = '1';
	tmp->count = right->count + left->count;
	tmp->right = right;
	tmp->left = left;
	tmp->next = left->next;

	struct List_node* tmp2 = list;
	
	while (tmp2->next != left) 
		tmp2 = tmp2->next;
	tmp2->next = tmp;

	while (list->next != right)
		list = list->next;
	list->next = right->next;

	left->next = tmp;
	right->next = tmp;

	return tmp;
}

void find_two_small_nodes (struct List_node* list, struct List_node** first_small_node, struct List_node** second_small_node) {
	struct List_node* tmp = list;
	(*first_small_node) = list;

	while (tmp != NULL) {
		if ((*first_small_node)->count > tmp->count) {
			(*first_small_node) = tmp;
		}
		tmp = tmp->next;
	}
	tmp = list;
	while (tmp != NULL) {
		printf ("%c - %d \n", tmp->c, tmp->count);
		tmp = tmp->next;
	}
	puts ("=======================================================");
	
	if ((*first_small_node) != list)
		(*second_small_node) = list;
	else
		(*second_small_node) = list->next;

	while (list != NULL) {
		if ((*second_small_node)->count > list->count && list != (*first_small_node)) {
			(*second_small_node) = list;
		}
		list = list->next;
	}
}

struct List_node* char_counting (char* str) {
	struct List_node* list = NULL;

	for (int i = 0; i < strlen (str); i++) {
		if (list == NULL) {
			list = creating_list (str[i]);
		}
		else if (find_char (list, str[i])) {
			add_list_node (list, str[i]);
		}
	}
	return list;
}

void add_list_node (struct List_node* list, char c) {
	struct List_node* tmp = (struct List_node*)malloc (sizeof (struct List_node));

	if (tmp == NULL) {
		puts ("Failed alloc memory");
		exit (EXIT_FAILURE);
	}
	tmp->c = c;
	tmp->count = 1;
	tmp->next = NULL;
	tmp->left = NULL;
	tmp->right = NULL;

	while (list->next != NULL)
		list = list->next;

	list->next = tmp;
}

 struct List_node* creating_list (char c) {
 	struct List_node* tmp = (struct List_node*)malloc (sizeof (struct List_node));
	
	if (tmp == NULL) {
		puts ("Failed alloc memory");
		exit (EXIT_FAILURE);
	}
	tmp->c = c;
	tmp->count = 1;
	tmp->next = NULL;
	tmp->left = NULL;
	tmp->right = NULL;

	return tmp;
}

int find_char (struct List_node* list, char c) {
	while (list != NULL) {
		if (list->c == c) {
			list->count++;
			return 0;
		}
		list = list->next;
	}
	return 1;
}

void free_list (struct List_node** list) {
	struct List_node* pl;

	while ((*list) != NULL) {
		pl = *list;
		*list = (*list)->next;
		free(pl);
	}
}