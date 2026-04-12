#include "dead_ends_finding.h"

// graph of cubes "dependencies" will form a directed graph without cycles -> we can do something
// may be use gray code to project cubes as several segments on gray array
// dont forget about cost, not only count
Cubeset dead_ends_finding_tree(Cubeset not_E, Cubeset not_E_covered) {
    Cubeset dead_end = EMPTY_SET;

    // todo implement

    return dead_end;
}

//todo code dublication
Cubeset* dead_ends_finding_bruteforce(Cubeset not_E, Cubeset not_E_covered) {
    // meet-in-the-middle can be implemented for x2 performance
    int min_cost = INT32_MAX;

    for (ull mask = 0; mask < (1ull << size(not_E)); mask++) {
        Cubeset selected = EMPTY_SET;
        int cost = 0;

        for (int i = 0; i < size(not_E); i++) {
            if (mask & (1ull << i)) {
                cost += cube_cost(not_E[i]) + 1;
                cubeset_add(selected, not_E[i]);
            }
        }

        for (int i = 0; i < size(not_E); i++)
            if (mask & (1ull << i))
                cost += cube_cost(not_E[i]) + 1;

        bool not_covering = false;
        for (int i = 0; i < size(not_E_covered); i++) {
            if (!cubeset_contains(selected, not_E_covered[i])) {
                not_covering = true;
                break;
            }
        }

        if (not_covering)
            continue;

        min_cost = min(min_cost, cost);
    }

    Cubeset* dead_ends = EMPTY_SET;
    for (ull mask = 0; mask < (1ull << size(not_E)); mask++) {
        Cubeset selected = EMPTY_SET;
        int cost = 0;

        for (int i = 0; i < size(not_E); i++) {
            if (mask & (1ull << i)) {
                cost += cube_cost(not_E[i]) + 1;
                cubeset_add(selected, not_E[i]);
            }
        }

        bool not_covering = false;
        for (int i = 0; i < size(not_E_covered); i++) {
            if (!cubeset_contains(selected, not_E_covered[i])) {
                not_covering = true;
                break;
            }
        }

        if (not_covering)
            continue;

        if (min_cost == cost) {
            set_push_force(dead_ends, selected);
        } else {
            set_free(selected);
        }
    }

    return dead_ends;
}