#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* huffman (char* str, char* decode_or_encode);

char* encode (char* str, char* result);

void decode (char* str, char* result);