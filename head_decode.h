#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// read code.txt and create char_code
struct char_code* read_code_tree (int* code_size, int* size_char_code);

void free_char_code_decode (struct char_code** str_code, int size_char_code);