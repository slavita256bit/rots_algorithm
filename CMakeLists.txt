cmake_minimum_required(VERSION 4.0)
project(rots_algorithm C)

set(CMAKE_C_STANDARD 23)

#add_compile_options(-DDEBUG)
add_compile_definitions(__STDC_WANT_LIB_EXT1__=1)

add_executable(rots_algorithm main.c
        cube.c
        cube.h
        io.c
        io.h
        set.c
        set.h
        rots_operations.c
        rots_operations.h
        utils.c
        utils.h
        rots_algorithm.c
        rots_algorithm.h
        checker.c
        checker.h
)
