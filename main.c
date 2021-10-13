/*
* @file main.c
* @author Renat Kagal <kagal@itspartner.net>
*
* Assembling : gcc -Wall main.c -pthread -o main
*
* Description : Huffman algo
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

#include "head.h"
#include "head_coder.h"

#define READING_SIZE 10

struct Node {
	char c;
	int count;

	Node* left;
	Node* right;
};

int main (int argc, char** argv) {
	char* path_in = "input.txt";
	char* path_out = "output.txt";
	char* decode_or_encode = "encode";

	if (argc > 1){
		if (argc > 2) 
			path_in = argv[1];
		if (argc > 3)
			path_out = argv[2];
		if (argv > 4)
			decode_or_encode = argv[3];
	}
	FILE* fp;
	
	if ((fp = fopen (path_in, "r")) == NULL) {
		puts ("Failed open input file");
		exit (EXIT_FAILURE);
	}
	char* str = malloc (sizeof (char));
	char buf[READING_SIZE];

	while (fgets (buf, READING_SIZE, fp) != EOF) {
		str = realloc (str, (strlen (str) + str(buf)) * sizeof (char));
		strcat (str, buf);
	}

	if (fclose (fp)) {
		puts ("Failed close input file");
		exit (EXIT_FAILURE);
	}
	str = huffman (str, decode_or_encode);

	if ((fp = fopen (path_out, "w+")) == NULL) {
		puts ("Failed open output file");
		exit (EXIT_FAILURE);
	}
	
	if (fprintf(fp, "%s", str) == EOF) {
		puts ("Failed print in output file");
		exit (EXIT_FAILURE);
	} 

	if (fp (close) ) {
		puts ("Failed close output file");
	}
}

char* huffman (char* str, char* decode_or_encode) {
	char* result;

	if (strcmp (str, "encode") == 0) {
		encode (str, result);
	}
	else {
		decode (str, result);
	}
	return result;
}