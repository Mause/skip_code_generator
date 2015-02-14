cmake_minimum_required(VERSION 2.8)
project(skip_code_generator)

add_subdirectory(lib/data_structures)
add_subdirectory(lib/data_structures/lib/dict.c)

include_directories(lib/data_structures/src/ll)
include_directories(lib/data_structures/src/arr)
include_directories(lib/data_structures/src/set)
include_directories(lib/data_structures/src/queue)
include_directories(lib/data_structures/lib/dict.c/src)

add_executable(skip_code_generator
    src/skip_code.c
)
target_link_libraries(
    skip_code_generator
    ll
    set
    arr
    dict
    queue
)
