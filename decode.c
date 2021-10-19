/*
* @file main.c
* @author Renat Kagal <kagal@itspartner.net>
*
* Assembling : gcc -Wall -c decode.c -o decode.o
*
* Description : decode Huffman algo
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

#include "head_decode.h"

#define READ_SIZE 32
#define CHAR_SIZE 8

struct char_code {
	char* code;
	char c;
};

void decode (char* str) {
	int code_size, size_char_code;
	struct char_code* str_code = read_code_tree (&code_size, &size_char_code);

	char* string = "\0";
	int tmp = 0;

	for (int i = 0, j = 1; i < code_size; i++) { 
		asprintf (&string, "%s%d", string, ((str[i / CHAR_SIZE] >> (8 - j)) & 1));
		if (j == 8)
			j = 0;
		j++;
	}
	puts (string);
	
	char* result = "\0";
	char* code = "\0";

	for (int i = 0; i < strlen (string); i++) {
		asprintf (&code, "%s%c", code, string[i]);
		for (int j = 0; j < size_char_code; j++) {
			puts (code);

			if (strcmp (code, str_code[j].code) == 0) {
				asprintf (&result, "%s%c", result, str_code[j].c);
				code = "\0";
			}
		}
	}
	puts (result);

	free_char_code_decode (&str_code, size_char_code);
}

struct char_code* read_code_tree (int* code_size, int* size_char_code) {
	FILE* fp;
	char c;
	char string[READ_SIZE];

	struct char_code* str_code = (struct char_code*)malloc (sizeof (struct char_code));

	if ((fp = fopen ("code.txt", "r")) == NULL) {
		puts ("Failed open code file");
		exit (EXIT_FAILURE);
	}

	fscanf (fp, "%d", code_size);
	fseek (fp, 1, SEEK_CUR);

	for ((*size_char_code) = 0; fscanf (fp, "%c%s", &c, string) != EOF; (*size_char_code)++){
		fseek (fp, 1, SEEK_CUR);

		if ((*size_char_code) == 0) {
			str_code[(*size_char_code)].c = c;
			str_code[(*size_char_code)].code = strdup (string);
		}
		else {
			str_code = realloc (str_code, sizeof (struct char_code) * ((*size_char_code) + 1));
			str_code[(*size_char_code)].c = c;
			str_code[(*size_char_code)].code = strdup (string);
		}
	}
	if (fclose (fp)) {
		puts ("Falied close file");
		exit (EXIT_FAILURE);
	}
	return str_code;
}

void free_char_code_decode (struct char_code** str_code, int size_char_code) {
	for (int i = 0; i < size_char_code; i++)
		free ((*str_code)[i].code);
	free (*str_code);
}