#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// choose to compress data or decompress 
char* huffman (char* str, char* decode_or_encode);

// compress data 
char* encode (char* str);

// decompress data
char* decode (char* str);