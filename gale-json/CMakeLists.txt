cmake_minimum_required (VERSION 3.5)
project (gale_test C)

if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ansi -pedantic -Wall")
endif()

add_library(gale_json gale-json.c)
add_executable(gale_test test.c)
target_link_libraries(gale_test gale_json)
