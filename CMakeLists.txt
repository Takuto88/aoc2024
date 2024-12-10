cmake_minimum_required(VERSION 3.10)
project(aoc_2024 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

add_library(liblists
        ${PROJECT_SOURCE_DIR}/src/libs/liblists.c
)
add_library(libreports
        ${PROJECT_SOURCE_DIR}/src/libs/libreports.c
)
add_library(libstring
        ${PROJECT_SOURCE_DIR}/src/libs/libstringutils.c
)

add_executable(aoc_2024 main.c src/day1/day1_1.c src/day1/day1_2.c
        src/day2/day2.h
        src/day2/day2.c
        src/day3/day3.h
        src/day3/day3.c
        src/day4/day4.h
        src/day4/day4.c
        src/day5/day5.h
        src/day5/day5.c)
target_include_directories(aoc_2024 PRIVATE
        ${PROJECT_SOURCE_DIR}/src/day1
        ${PROJECT_SOURCE_DIR}/src/day2
        ${PROJECT_SOURCE_DIR}/src/day3
        ${PROJECT_SOURCE_DIR}/src/day4
        ${PROJECT_SOURCE_DIR}/src/day5
        ${PROJECT_SOURCE_DIR}/src/libs
)
target_link_libraries(aoc_2024 liblists libreports libstring)
