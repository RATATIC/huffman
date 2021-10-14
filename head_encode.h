#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct List_node* char_counting (char* str);

int find_char (struct List_node* list, char c);

struct List_node* creating_list(char c);

void add_list_node (struct List_node* list, char c);

void free_list (struct List_node** list);

struct List_node* make_tree (struct List_node* list);

void find_two_small_nodes (struct List_node* list, struct List_node** first_small_node, struct List_node** second_small_node);

struct List_node* creating_union_of_two_small_nodes (struct List_node* list, struct List_node* right, struct List_node* left);

struct char_code* creating_char_code (struct List_node* list, int size);