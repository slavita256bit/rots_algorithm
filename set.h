#pragma once
#include "cube.h"

#define MIN_CAPACITY 2

struct Set {
    int size;
    int capacity;
    Cube *list;
};
typedef struct Set Set;

extern const Set EMPTY_SET;

Set set_init();
Set* set_init_ptr();
Set set_init_with(Cube cube);
Set* set_copy(Set *set);

Set set_read(FILE *file, int n, int cube_length);
void set_print(FILE *file, Set* set);

Set* set_union(Set *a, Set *b);
bool set_contains(Set *set, Cube cube);
void set_add(Set *set, Cube cube);
void set_add_all(Set *to, Set *from);
Set* set_subtract(Set *a, Set *b);

void set_free(Set set);
void set_free_ptr(Set *set);
