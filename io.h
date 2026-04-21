#pragma once
#include <stdint.h>
#include <stdio.h>

#define MAX_VAR_LENGTH 42

void flush_input(FILE *file);
int read_int(FILE *file);
char** var_names_read(FILE *file, int cube_length);