cmake_minimum_required(VERSION 4.0)
project(rots_algorithm C)

set(CMAKE_C_STANDARD 23)

add_compile_definitions(__STDC_WANT_LIB_EXT1__=1)

add_executable(rots_algorithm main.c
        cube.c
        cube.h
        io.c
        io.h
        set.c
        set.h
        rot_operations.c
        rot_operations.h
        utils.c
        utils.h
)
