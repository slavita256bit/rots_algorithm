#pragma once
#include "set.h"

// Cubeset -> Cubes array (Cube* with insane shadow header)
typedef Cube* Cubeset;

// todo optimize contains (with # ?)
#define cubeset_add(set, cube_exr) do {            \
        Cube _cube = (cube_exr);                   \
        if (cube_is_invalid(_cube)) break;         \
        if (cubeset_contains(set, _cube)) break;   \
        set_push_force(set, _cube);                \
    } while (0);                                   \


#define cubeset_add_all(to, from) do {                \
        for (int _i = 0; _i < size(from); _i++)       \
            cubeset_add(to, from[_i]);                \
    } while (0);                                      \

Cubeset cubeset_init_with(Cube cube);

Cubeset cubeset_read(FILE *file, int n, int cube_length);
void cubeset_print(FILE *file, Cubeset set);

bool cubeset_contains(Cubeset set, Cube cube);

void cubeset_sort(Cubeset set);
Cubeset cubeset_subtract(Cubeset a, Cubeset b);
Cubeset cubeset_union(Cubeset a, Cubeset b);
