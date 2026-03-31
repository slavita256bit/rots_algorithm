#include "cubeset.h"

Cubeset cubeset_init_with(Cube cube) {
    Cubeset set = EMPTY_SET;
    set_push_force(set, cube);
    return set;
}

// todo: think about faster approach
// todo idea: implement this with # operation (this approach can also make main algo shorter (if we will give some feedback))
bool cubeset_contains(Cubeset set, Cube cube) {
    for (int i = 0; i < size(set); i++)
        if (cube_contains(set[i], cube))
            return true;
    return false;
}

// union with deep check | todo: optimize (for example with pre-sorted a and b and mergesort)
Cubeset cubeset_union(Cubeset a, Cubeset b) {
    Cubeset result = EMPTY_SET;

    int n = 0;
    if (!is_empty(a)) n = max(n, a[0].n);
    if (!is_empty(b)) n = max(n, b[0].n);

    for (int x_count = n; x_count >= 0; x_count--) {
        for (int i = 0; i < size(a); i++)
            if (cube_x_count(a[i]) == x_count)
                cubeset_add(result, a[i]);

        for (int i = 0; i < size(b); i++)
            if (cube_x_count(b[i]) == x_count)
                cubeset_add(result, b[i]);
    }

    return result;
}

Cubeset cubeset_subtract(Cubeset a, Cubeset b) {
    Cubeset result = EMPTY_SET;

    for (int i = 0; i < size(a); i++)
        if (!cubeset_contains(b, a[i])) // todo optimize (with # ?)
            cubeset_add(result, a[i]);

    return result;
}

Cubeset cubeset_read(FILE *file, int n, int cube_length) {
    Cubeset set = EMPTY_SET;

    for (int i = 0; i < n; i++)
        cubeset_add(set, cube_read(file, cube_length));

    return set;
}

void cubeset_print(FILE *file, Cubeset set) {
    for (int i = 0; i < size(set); i++) {
        cube_print(file, set[i]);
        printf("\n");
    }
}
