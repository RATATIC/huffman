#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// create List_node list of char in str with there count  
struct List_node* char_counting (char* str);

// find char in list and increment node
int find_char (struct List_node* list, char c);

// create top of list
struct List_node* creating_list(char c);

// add in list new node
void add_list_node (struct List_node* list, char c);

// create code tree 
struct List_node* make_tree (struct List_node* list);

// find the two smallest nodes in list
void find_two_small_nodes (struct List_node* list, struct List_node** first_small_node, struct List_node** second_small_node);

// create new node which union two node
struct List_node* creating_union_of_two_small_nodes (struct List_node* list, struct List_node* right, struct List_node* left);

// create new node which one top 
struct List_node* change_top_and_creating_union (struct List_node** list, struct List_node* first_small_node, struct List_node* second_small_node);

// create char_code 
struct char_code* creating_char_code (struct List_node** cpy_list, int size);

void free_tree (struct List_node** tree);

void free_char_code(struct char_code* str_code, int size_char_code);

// write code of char in file
void wrie_in_char_code (struct char_code* str_code, int size_char_code);