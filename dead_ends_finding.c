#include "dead_ends_finding.h"

// graph of cubes "dependencies" will form a tree -> we can create dp (dfs) that will cover optimally
// dont forget about cost, not only count
Set* dead_ends_finding_tree(Set *not_E, Set not_E_covered) {
    Set* dead_end = nullptr;

    //todo

    return dead_end;
}

// just bruteforcing all combinations instead of if algorithm
Set* dead_ends_finding_bruteforce(Set *not_E, Set not_E_covered) {
    // meet-in-the-middle can be implemented for x2 performance
    int min_cost = INT32_MAX;
    Set* dead_end = nullptr;
    for (ull mask = 0; mask < (1ull << not_E->size); mask++) {
        Set* selected = set_init_ptr();
        int cost = 0;

        for (int i = 0; i < not_E->size; i++) {
            if (mask & (1ull << i)) {
                cost += cube_cost(&not_E->list[i]) + 1;
                set_add(selected, not_E->list[i]);
            }
        }

        bool not_covering = false;
        for (int i = 0; i < not_E_covered.size; i++) {
            if (!set_contains(selected, not_E_covered.list[i])) {
                not_covering = true;
                break;
            }
        }

        if (not_covering)
            continue;

        if (min_cost >= cost) {
            min_cost = cost;
            set_free_ptr(dead_end);
            dead_end = selected;
        } else {
            set_free_ptr(selected);
        }
    }
    // todo print all minimal dead ends?

    return dead_end;
}