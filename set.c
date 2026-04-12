#include "set.h"

// Implemented this way to make IDE show some warnings correctly
inline int size(const void* arr) {
    if (is_empty(arr)) return 0;
    return ((const Header*)arr - 1)->size;
}