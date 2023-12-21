cmake_minimum_required (VERSION 3.21)

add_executable(aoc18 aoc18.cpp)
target_include_directories(aoc18 PUBLIC ../common)
target_link_libraries(aoc18 PRIVATE ctre::ctre)

add_executable(aoc18_1 aoc18_1.cpp)
target_include_directories(aoc18_1 PUBLIC ../common)
target_link_libraries(aoc18_1 PRIVATE ctre::ctre)

add_executable(aoc18_2 aoc18_2.cpp)
target_include_directories(aoc18_2 PUBLIC ../common)
target_link_libraries(aoc18_2 PRIVATE ctre::ctre)