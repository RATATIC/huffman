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

void print_bits (char n) {
    char* bits = "";

    while (n > 0) {
        asprintf (&bits, "%d%s", (n & 1), bits);
        n = n >> 1;
    }
    printf ("%s\n", bits);
}

char* encode (char* str) {
	struct List_node* list = char_counting (str);
	struct List_node* pl = list;

	int size_char_code = 0;
	while (pl != NULL) {
		size_char_code++;
		pl = pl->next;
	}
	struct List_node** cpy_list = (struct List_node**)malloc (sizeof (struct List_node*) * size_char_code);
	
	if (cpy_list == NULL) {
		puts ("Failed alloc memory");
		exit (EXIT_FAILURE);
	}
	pl = list;
	
	for (int i = 0; i < size_char_code; i++) {
		cpy_list[i] = pl;
		pl = pl->next; 
	}
	struct List_node* tree = make_tree (list);
	struct char_code* str_code = creating_char_code (cpy_list, size_char_code);

	for (int i = 0; i < size_char_code; i++) {
		printf ("%c - %s\n", str_code[i].c, str_code[i].code);
	}
	char* string = "\0";

	for (int i = 0; i < strlen (str); i++) {
		for (int j = 0; j < size_char_code; j++) {
			if (str[i] == str_code[j].c) {
				asprintf (&string, "%s%s", string, str_code[j].code);
			}
		}
	}
	char* result = "\0";
	char c = 0;

	for (int i = 0, j = 1; i < strlen (string); i++) {
		if (string[i] == '0')
			c &= ~(1 << (8 - j));
		else{
			c |= (1 << (8 - j));
		}

		if (j == 8) {
			//print_bits (c);
			asprintf (&result, "%s%c", result, c);
			j = 0;
			c = 0;
		}
		j++;
	}

	printf ("%s\n", result);

	write_in_char_code (str_code, size_char_code, strlen (string));

	free_tree (&tree);
	free_char_code (str_code, size_char_code);
	free (cpy_list);
	free (string);

	return result;
}

// write code of char in file
void write_in_char_code (struct char_code* str_code, int size_char_code, int string_size) {
	FILE* fp;

	if ((fp = fopen ("code.txt", "w")) == NULL) {
		puts ("Failed open code.txt");
		exit (EXIT_FAILURE);
	}
	fprintf (fp, "%d\n", string_size);

	for (int i = 0; i < size_char_code; i++)
		fprintf(fp, "%c %s\n", str_code[i].c, str_code[i].code);

	if (fclose (fp)) {
		puts ("Failed close file");
		exit (EXIT_FAILURE);
	}
}

// create char_code 
struct char_code* creating_char_code (struct List_node** cpy_list, int size_char_code) {
	struct char_code* tmp = (struct char_code*)malloc (sizeof (struct char_code) * size_char_code);
	
	if (tmp == NULL) {
		puts ("Failed alloc memory char_code");
		exit (EXIT_FAILURE);
	}
	for (int i = 0; i < size_char_code; i++) {
		tmp[i].c = cpy_list[i]->c;
		tmp[i].code = "\0";
		while (cpy_list[i]->next != NULL) {
			if (cpy_list[i]->next->left == cpy_list[i]) {
				asprintf (&tmp[i].code, "%c%s", '0', tmp[i].code);
			}
			else {
				asprintf (&tmp[i].code, "%c%s", '1', tmp[i].code);
			}
			cpy_list[i] = cpy_list[i]->next;
		}
	}
	return tmp;
}

// create code tree 
struct List_node* make_tree (struct List_node* list) {
	struct List_node* tmp = NULL;
	struct List_node* first_small_node = NULL;
	struct List_node* second_small_node = NULL;

	while(list->next != NULL) {
		find_two_small_nodes (list, &first_small_node, &second_small_node);

		if (first_small_node != list && second_small_node != list)
			tmp = creating_union_of_two_small_nodes (list, first_small_node, second_small_node);
		else 
			tmp = change_top_and_creating_union (&list, first_small_node, second_small_node);
	}
	return tmp;
}

// create new node which one top
struct List_node* change_top_and_creating_union (struct List_node** list, struct List_node* right, struct List_node* left) {
	struct List_node* tmp = (struct List_node*)malloc (sizeof (struct List_node));
	struct List_node* tmp2 = (*list);

	if (tmp == NULL) {
		puts ("Failed alloc memory for tmp in creating_union_of_two_small_nodes");
		exit(EXIT_FAILURE);
	}
	tmp->c = '1';
	tmp->count = right->count + left->count;
	tmp->right = right;
	tmp->left = left;

	if (right == (*list)) {
		while (tmp2->next != left)
			tmp2 = tmp2->next;
		tmp2->next = left->next;

		tmp->next = right->next;
	}
	else {
		while (tmp2->next != right)
			tmp2 = tmp2->next;
		tmp2->next = right->next;
		
		tmp->next = left->next;
	}	
	(*list) = tmp;

	left->next = tmp;
	right->next = tmp;
	
	return tmp;
}

// create new node which is the union of the two smallest nodes
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

// find the two smallest nodes in list
void find_two_small_nodes (struct List_node* list, struct List_node** first_small_node, struct List_node** second_small_node) {
	struct List_node* tmp = list;
	(*first_small_node) = list;

	while (tmp != NULL) {
		if ((*first_small_node)->count > tmp->count) {
			(*first_small_node) = tmp;
		}
		tmp = tmp->next;
	}

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

// create List_node list of char from str with their count 
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
// add in list new node
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

// create top of list
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

// find char in list and increment count in node
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

void free_tree (struct List_node** tree) {
	if ((*tree)->right != NULL)
		free_tree (&((*tree)->right));
	if ((*tree)->left != NULL)
		free_tree (&((*tree)->left));
	free ((*tree));
}

void free_char_code(struct char_code* str_code, int size_char_code) {
	for (int i = 0 ; i < size_char_code; i++)
		free(str_code[i].code);
	free (str_code);
}