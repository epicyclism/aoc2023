cmake_minimum_required (VERSION 3.21)

add_executable(aoc19 aoc19.cpp)
target_include_directories(aoc19 PUBLIC ../common)
target_link_libraries(aoc19 PRIVATE ctre::ctre)

add_executable(aoc19_o aoc19_o.cpp)
target_include_directories(aoc19_o PUBLIC ../common)
target_link_libraries(aoc19_o PRIVATE ctre::ctre)