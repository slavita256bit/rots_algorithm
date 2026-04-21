#pragma once
#include "cube.h"

#define MIN_CAPACITY 2

// works for every type
// header is shadowed before array
// limitations:
// every function that changes length of array should bw wrapped with define to make sure address of array changed

struct Header {
    int size;
    int capacity;
};
typedef struct Header Header;

#define EMPTY_SET nullptr

#define get_header(set) ((Header*)(set) - 1)

#define set_push_force(set, x)                                                          \
    do {                                                                                \
        if ((set) == EMPTY_SET) {                                                         \
            Header *header = malloc(sizeof(*(set)) * MIN_CAPACITY + sizeof(Header));      \
            header->size = 0;                                                           \
            header->capacity = MIN_CAPACITY;                                            \
            (set) = (void*)(header + 1);                                                  \
        }                                                                               \
        Header *header = get_header((set));                                               \
        if (header->size >= header->capacity) {                                         \
            header->capacity *= 2;                                                      \
            header = realloc(header, sizeof(*(set)) * header->capacity + sizeof(Header)); \
            (set) = (void*)(header + 1);                                                  \
        }                                                                               \
        (set)[header->size++] = (x);                                                    \
    } while(0)

#define set_clone(new_set, old_set)                                                        \
    do {                                                                                   \
        set_free(new_set);                                                                 \
        if (is_empty(old_set)) break;                                                      \
        int item_size = sizeof(*old_set);                                                  \
        Header *old_header = get_header(old_set);                                          \
        int whole_size = item_size * old_header->capacity + sizeof(Header);                \
        Header *new_header = malloc(whole_size);                                           \
        memcpy(new_header, old_header, whole_size);                                        \
        new_set = (void*)(new_header + 1);                                                 \
    } while(0)

#define is_empty(set) ((set) == EMPTY_SET)

#define set_free(set) (is_empty(set) ? 0 : free(get_header(set)))

extern inline int size(const void* set);

