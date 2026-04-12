#include "cubeset.h"

Cubeset cubeset_init_with(Cube cube) {
    Cubeset set = EMPTY_SET;
    set_push_force(set, cube);
    return set;
}

// todo: think where this really needed
bool cubeset_contains(Cubeset set, Cube cube) {
    for (int i = 0; i < size(set); i++)
        if (cube_contains(set[i], cube))
            return true;
    return false;
}

// is a < b
// sort by 1.mask of x (lefter -> larger x count), 2.mask of defaults
bool sort_comp(Cube a, Cube b) {
    ull special_a_count = cube_x_count(a);
    ull special_b_count = cube_x_count(b);
    if (special_a_count != special_b_count)
        return special_a_count > special_b_count;

    ull special_a = cube_special_only(a);
    ull special_b = cube_special_only(b);
    if (special_a != special_b)
        return special_a < special_b;

    return cube_default_only(a) < cube_default_only(b);
};

void cubeset_sort(Cubeset set) {
    sort(set, 0, size(set) - 1, sort_comp);
}

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

//todo
// union with deep check | optimized (pre-sorted a and b and mergesort)
// Cubeset cubeset_union(Cubeset a, Cubeset b) {
//     Cubeset result = EMPTY_SET;
//
//     int k = 0, i = 0, j = 0;
//     int n = size(a);
//     int m = size(b);
//     while (i < n || j < m) {
//         //
//     }
//     // have to make bor?
//
//     return result;
// }

// todo this is quite similar to z#(Z − z), so lets add arg flag to make function lighter and may be faster with pre-sort?
Cubeset cubeset_subtract(Cubeset a, Cubeset b) {
    Cubeset result = EMPTY_SET;

    for (int i = 0; i < size(a); i++)
        if (!cubeset_contains(b, a[i]))
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
