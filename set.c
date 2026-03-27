#include "set.h"

const Set EMPTY_SET = {0, 0, nullptr};

Set set_init() {
    Set set;
    set.size = 0;
    set.capacity = MIN_CAPACITY;
    set.list = calloc(set.capacity, sizeof(Cube));
    return set;
}

Set* set_init_ptr() {
    Set *set = malloc(sizeof(Set));
    set->size = 0;
    set->capacity = MIN_CAPACITY;
    set->list = calloc(set->capacity, sizeof(Cube));
    return set;
}

Set set_init_with(Cube cube) {
    Set set = set_init();
    set_add(&set, cube);
    return set;
}

void set_free(Set set) {
    free(set.list);
}

void set_free_ptr(Set *set) {
    if (set == NULL)
        return;

    free(set->list);
    free(set);
}

// todo: think about faster approach
// todo idea: implement this with # operation (this approach can also make main algo shorter (if we will give some feedback))
bool set_contains(Set *set, Cube cube) {
    for (int i = 0; i < set->size; i++)
        if (cube_contains(&set->list[i], &cube))
            return true;
    return false;
}

void set_add(Set *set, Cube cube) {
    if (cube_is_invalid(&cube))
        return;

    if (set_contains(set, cube)) // todo optimize
        return;

    if (set->size == set->capacity) {
        set->capacity *= 2; // todo 2 -> 1.5?
        Cube* list = realloc(set->list, set->capacity * sizeof(Cube));
        if (list == NULL) {printf("CANNOT ALLOCATE MEMORY!"); exit(-1);}
        set->list = list;
    }

    set->list[set->size++] = cube;
}

// union with deep check | todo: optimize (for example with pre-sorted a and b and mergesort)
Set* set_union(Set *a, Set *b) {
    Set *result = set_init_ptr();

    int n = 0;
    if (a->size > 0) n = max(n, a->list[0].n);
    if (b->size > 0) n = max(n, b->list[0].n);

    for (int x_count = n; x_count >= 0; x_count--) {
        for (int i = 0; i < a->size; i++)
            if (cube_x_count(&a->list[i]) == x_count)
                set_add(result, a->list[i]);

        for (int i = 0; i < b->size; i++)
            if (cube_x_count(&b->list[i]) == x_count)
                set_add(result, b->list[i]);
    }

    return result;
}

Set* set_subtract(Set *a, Set *b) {
    Set* result = set_init_ptr();

    for (int i = 0; i < a->size; i++)
        if (!set_contains(b, a->list[i]))
            set_add(result, a->list[i]);

    return result;
}

Set* set_copy(Set *set) {
    Set* result = set_init_ptr();

    for (int i = 0; i < set->size; i++)
            set_add(result, set->list[i]);

    return result;
}

void set_add_all(Set *to, Set *from) {
    for (int i = 0; i < from->size; i++)
        set_add(to, from->list[i]);
}

Set set_read(FILE *file, int n, int cube_length) {
    Set set = set_init();
    for (int i = 0; i < n; i++)
        set_add(&set, cube_read(file, cube_length));
    return set;
}

void set_print(FILE *file, Set* set) {
    for (int i = 0; i < set->size; i++) {
        cube_print(file, &set->list[i]);
        printf("\n");
    }
}
