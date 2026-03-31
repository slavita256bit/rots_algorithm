#include "rots_algorithm.h"
#include <assert.h>
#include <stdio.h>

Cubeset rots_algorithm(const char *filename) {
    FILE *file = fopen(filename, "r");

    // printf("Write variables count (<=32): ");
    int cube_length = read_int(file);
    // printf("Write L (f=1) length count: ");
    int l_len = read_int(file);
    // printf("Write L (f=1) cubes:\n");
    Cubeset L = cubeset_read(file, l_len, cube_length);
    // printf("Write N (f=*) length count: ");
    int n_len = read_int(file);
    // printf("Write N (f=*) cubes:\n");
    Cubeset N = cubeset_read(file, n_len, cube_length);

    fclose(file);

    Cubeset Z = EMPTY_SET; // union of all Z_i

    Cubeset last_A = EMPTY_SET; set_clone(last_A, L);
    Cubeset last_B = EMPTY_SET; set_clone(last_B, N);

    int iterations = 0;
    while (!is_empty(last_A)) {
        Cubeset C = cubeset_union(last_A, last_B);
        Cubeset A = EMPTY_SET; // cubes multiplied
        Cubeset Z_i = EMPTY_SET; // cubes not multiplied

        bool *used = calloc(size(C), sizeof(bool));

        // printf("{===== %d", iterations);
        for (int i = 0; i < size(C); i++) { // Ci * Ci
            for (int j = i + 1; j < size(C); j++) {
                Cube cube = rot_multiply(C[i], C[j]);
                // printf("\n");
                // cube_print(&cube);
                cube = cube_convert_y2x(cube);

                if (!cube_is_invalid(cube) && cube_x_count(cube) > max(cube_x_count(C[i]), cube_x_count(C[j]))) {
                    used[i] = used[j] = true;
                    cubeset_add(A, cube);
                }
            }
        }
        // printf("\n}===== %d", iterations);

        for (int i = 0; i < size(C); i++)
            if (!used[i])
                cubeset_add(Z_i, C[i]);

        Cubeset B = cubeset_subtract(C, Z_i); // cubes left

        cubeset_add_all(Z, Z_i);

#ifdef DEBUG
        printf("\nC%d:\n", iterations);
        cubeset_print(stdout, C);
        printf("\nZ%d:\n", iterations);
        cubeset_print(stdout, Z_i);
        printf("\nA%d:\n", iterations + 1);
        cubeset_print(stdout, A);
        printf("\nB%d:\n", iterations + 1);
        cubeset_print(stdout, B);
#endif

        iterations++;

        set_free(last_A);
        set_free(last_B);
        set_free(C);
        last_A = A;
        last_B = B;

        set_free(Z_i);
        free(used);
    }

    Cubeset ZZZ = EMPTY_SET;

    for (int i = 0; i < size(Z); i++) { // ZZZ = z#(Z − z) // cubes that left
        Cubeset cubes = EMPTY_SET;
        cubeset_add(cubes, Z[i]);

        for (int j = 0; j < size(Z); j++) {
            if (i == j) continue;

            Cubeset cubes_next = EMPTY_SET;

            for (int k = 0; k < size(cubes); k++) {
                Cubeset result = rot_subtract(cubes[k], Z[j]);
                cubeset_add_all(cubes_next, result);
                set_free(result);
            }

            set_free(cubes);
            cubes = cubes_next;
        }

        cubeset_add_all(ZZZ, cubes);

        set_free(cubes);
    }

    Cubeset E = EMPTY_SET; // core of the function
    for (int i = 0; i < size(ZZZ); i++) { // E = ZZZ ∩ L
        bool cube_is_core = false;

        for (int j = 0; j < size(L); j++) {
            Cube result = rot_union(ZZZ[i], L[j]);
            if (!cube_is_invalid(result)) {
                cube_is_core = true;
                break;
            }
        }

        if (cube_is_core) {
            bool found_related = false;
            for (int j = 0; j < size(Z); j++) { // finding Z cube that related to this | todo maybe not optimal? Z cubes and ZZZ should be linked? | todo optimize
                if (cube_contains(Z[j], ZZZ[i])) {
                    cubeset_add(E, Z[j]);
                    found_related = true;
                    break;
                }
            }
            assert(found_related);
        }
    }

    Cubeset not_E_covered = EMPTY_SET; // not covered by core (L1 in book)
    for (int i = 0; i < size(L); i++) { // not_E_coveted = L#Е
        Cubeset cubes = EMPTY_SET;
        cubeset_add(cubes, L[i]);

        for (int j = 0; j < size(E); j++) {
            Cubeset cubes_next = EMPTY_SET;
            for (int k = 0; k < size(cubes); k++) {
                Cubeset result = rot_subtract(cubes[k], E[j]);
                cubeset_add_all(cubes_next, result);
                set_free(result);
            }

            set_free(cubes);
            cubes = cubes_next;
        }

        cubeset_add_all(not_E_covered, cubes);

        set_free(cubes);
    }

    Cubeset not_E = cubeset_subtract(Z, E); // Z cubes that not in E (Z^ = Z\E from book)

    Cubeset min_function = EMPTY_SET;
    cubeset_add_all(min_function, E);

    // bad algorithm
    // for (int i = 0; i < not_E_covered.size; i++) { // this actually works? there is not enough info for common solution in book
    //     Cube perfect_cube = INVALID_CUBE;
    //     for (int j = 0; j < not_E->size; j++) {
    //         if (cube_contains(&not_E->list[j], &not_E_covered.list[i])) { // maybe optimize with pre-sorted by X count not_E
    //             perfect_cube = max_related(cube_x_count(&perfect_cube), cube_x_count(&not_E->list[j]), perfect_cube, not_E->list[j]);
    //         }
    //     }
    //     assert(!cube_is_invalid(&perfect_cube));
    //     set_add(mdnf, perfect_cube);
    // }

    // my idea instead of IF algorithm
    // last step - covering L1 (not_E_covered) with some Z^ (not_E)

    Cubeset dead_end = dead_ends_finding_bruteforce(not_E, not_E_covered);
    // Set* dead_end = dead_ends_finding_tree(not_E, not_E_covered); // make bool arg for this

    cubeset_add_all(min_function, dead_end);

#ifdef DEBUG
    printf("\nZ:\n");
    cubeset_print(stdout, Z);

    printf("\nZZZ (z#(Z − z)):\n");
    cubeset_print(stdout, ZZZ);

    // printf("\nL:\n");
    // set_print(&L);

    printf("\nE:\n");
    cubeset_print(stdout, E);

    printf("\nL_1:\n");
    cubeset_print(stdout, not_E_covered);

    printf("\nZ^:\n");
    cubeset_print(stdout, not_E);

    printf("\nf_mdnf:\n");
    cubeset_print(stdout, min_function);
#endif

    set_free(last_A);
    set_free(last_B);
    set_free(not_E);
    set_free(dead_end);
    set_free(N);
    set_free(L);
    set_free(Z);
    set_free(ZZZ);
    set_free(E);
    set_free(not_E_covered);

    return min_function;
}

// todo tables output for course project
