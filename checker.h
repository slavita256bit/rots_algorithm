#pragma once
#include "rots_operations.h"

#define MAX_COMBINATIONS_COUNT 24

bool verify_function(const char *filename, Cubeset optimized_function);
void generate_test(const char *filename);