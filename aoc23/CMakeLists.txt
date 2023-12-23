cmake_minimum_required (VERSION 3.21)

add_executable(aoc23 aoc23.cpp)
target_include_directories(aoc23 PUBLIC ../common)

add_executable(aoc23_bfs aoc23_bfs.cpp)
target_include_directories(aoc23_bfs PUBLIC ../common)
