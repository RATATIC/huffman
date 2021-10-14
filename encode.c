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
	struct List_node* tree = make_tree (list);

	pl = list;
	int size_char_code = 0;

	while (pl != NULL) {
		size_char_code++;
		pl = pl->next;
	}
	struct char_code* top = creating_char_code (list, size_char_code);

	free_list (&list);
}

struct char_code* creating_char_code (struct List_node* list, int size) {

}

struct List_node* make_tree (struct List_node* list) {
	struct List_node* tmp = NULL;
	struct List_node* first_small_node = NULL;
	struct List_node* second_small_node = NULL;
	
	while(list->next != NULL) {
		find_two_small_nodes (list, &first_small_node, &second_small_node);
		tmp = creating_union_of_two_small_nodes (list, first_small_node, second_small_node);
	}

	return tmp;
}

struct List_node* creating_union_of_two_small_nodes (struct List_node* list, struct List_node* right, struct List_node* left) {
	struct List_node* tmp = (struct List_node*)malloc (sizeof (struct List_node));

	if (tmp == NULL) {
		puts ("Failed alloc memory for tmp in creating_union_of_two_small_nodes");
		exit(EXIT_FAILURE);
	}
	tmp->c = '\0';
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
	(*second_small_node) = list->next;

	while (tmp != NULL) {
		if ((*first_small_node)->count < tmp->count) {
			(*first_small_node) = tmp;
		}
		tmp = tmp->next;
	}

	while (list != NULL) {
		if ((*second_small_node)->count < list->count && list != (*first_small_node)) {
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