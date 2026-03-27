#include "rots_algorithm.h"
#include <assert.h>
#include <stdio.h>

Set* rots_algorithm(const char *filename) {
    FILE *file = fopen(filename, "r");

    // printf("Write variables count (<=32): ");
    int cube_length = read_int(file);
    // printf("Write L (f=1) length count: ");
    int l_len = read_int(file);
    // printf("Write L (f=1) cubes:\n");
    Set L = set_read(file, l_len, cube_length);
    // printf("Write N (f=*) length count: ");
    int n_len = read_int(file);
    // printf("Write N (f=*) cubes:\n");
    Set N = set_read(file, n_len, cube_length);

    fclose(file);

    Set Z = set_init(); // union of all Z_i

    Set *last_A = set_copy(&L);
    Set *last_B = set_copy(&N);
    int iterations = 0;
    while (last_A->size > 0) {
        Set *C = set_union(last_A, last_B);
        Set *A = set_init_ptr(); // cubes multiplied
        Set Z_i = set_init(); // cubes not multiplied

        bool *used = calloc(C->size, sizeof(bool));

        // printf("{===== %d", iterations);
        for (int i = 0; i < C->size; i++) { // Ci * Ci
            for (int j = i + 1; j < C->size; j++) {
                Cube cube = rot_multiply(&C->list[i], &C->list[j]);
                // printf("\n");
                // cube_print(&cube);
                cube_convert_y2x(&cube);

                if (!cube_is_invalid(&cube) && cube_x_count(&cube) > max(cube_x_count(&C->list[i]), cube_x_count(&C->list[j]))) {
                    used[i] = used[j] = true;
                    set_add(A, cube);
                }
            }
        }
        // printf("\n}===== %d", iterations);

        for (int i = 0; i < C->size; i++)
            if (!used[i])
                set_add(&Z_i, C->list[i]);

        Set *B = set_subtract(C, &Z_i); // cubes left

        set_add_all(&Z, &Z_i);

#ifdef DEBUG
        printf("\nC%d:\n", iterations);
        set_print(C);
        printf("\nZ%d:\n", iterations);
        set_print(&Z_i);
        printf("\nA%d:\n", iterations + 1);
        set_print(A);
        printf("\nB%d:\n", iterations + 1);
        set_print(B);
#endif

        iterations++;

        set_free_ptr(last_A);
        set_free_ptr(last_B);
        set_free_ptr(C);
        last_A = A;
        last_B = B;

        set_free(Z_i);
        free(used);
    }

    Set ZZZ = set_init();

    for (int i = 0; i < Z.size; i++) { // ZZZ = z#(Z − z) // cubes that left
        Set* cubes = set_init_ptr();
        set_add(cubes, Z.list[i]);

        for (int j = 0; j < Z.size; j++) {
            if (i == j) continue;

            Set* cubes_next = set_init_ptr();

            for (int k = 0; k < cubes->size; k++) {
                Set result = rot_subtract(&cubes->list[k], &Z.list[j]);
                set_add_all(cubes_next, &result);
                set_free(result);
            }

            free(cubes);
            cubes = cubes_next;
        }

        set_add_all(&ZZZ, cubes);

        set_free_ptr(cubes);
    }

    Set E = set_init(); // core of the function
    for (int i = 0; i < ZZZ.size; i++) { // E = ZZZ ∩ L
        bool cube_is_core = false;

        for (int j = 0; j < L.size; j++) {
            Cube result = rot_union(&ZZZ.list[i], &L.list[j]);
            if (!cube_is_invalid(&result)) {
                cube_is_core = true;
                break;
            }
        }

        if (cube_is_core) {
            bool found_related = false;
            for (int j = 0; j < Z.size; j++) { // finding Z cube that related to this | todo maybe not optimal? Z cubes and ZZZ should be linked? | todo optimize
                if (cube_contains(&Z.list[j], &ZZZ.list[i])) {
                    set_add(&E, Z.list[j]);
                    found_related = true;
                    break;
                }
            }
            assert(found_related);
        }
    }

    Set not_E_covered = set_init(); // not covered by core (L1 in book)
    for (int i = 0; i < L.size; i++) { // not_E_coveted = L#Е
        Set *cubes = set_init_ptr();
        set_add(cubes, L.list[i]);

        for (int j = 0; j < E.size; j++) {
            Set *cubes_next = set_init_ptr();
            for (int k = 0; k < cubes->size; k++) {
                Set result = rot_subtract(&cubes->list[k], &E.list[j]);
                set_add_all(cubes_next, &result);
                set_free(result);
            }

            set_free_ptr(cubes);
            cubes = cubes_next;
        }

        set_add_all(&not_E_covered, cubes);

        set_free_ptr(cubes);
    }

    Set *not_E = set_subtract(&Z, &E); // Z cubes that not in E (Z^ = Z\E from book)

    Set* min_function = set_init_ptr();
    set_add_all(min_function, &E);

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

    Set* dead_end = dead_ends_finding_bruteforce(not_E, not_E_covered);
    // Set* dead_end = dead_ends_finding_tree(not_E, not_E_covered);

    set_add_all(min_function, dead_end);

#ifdef DEBUG
    printf("\nZ:\n");
    set_print(&Z);

    printf("\nZZZ (z#(Z − z)):\n");
    set_print(&ZZZ);

    // printf("\nL:\n");
    // set_print(&L);

    printf("\nE:\n");
    set_print(&E);

    printf("\nL_1:\n");
    set_print(&not_E_covered);

    printf("\nZ^:\n");
    set_print(not_E);

    printf("\nf_mdnf:\n");
    set_print(min_function);
#endif

    set_free_ptr(last_A);
    set_free_ptr(last_B);
    set_free_ptr(not_E);
    set_free_ptr(dead_end);

    set_free(N);
    set_free(L);
    set_free(Z);
    set_free(ZZZ);
    set_free(E);
    set_free(not_E_covered);

    return min_function;
}

// todo tables output for course project
// todo think about making everything with links (*) to make everything equal (without ., only ->)? or make everything with dots