cmake_minimum_required (VERSION 3.21)

add_executable(aoc12 aoc12.cpp)
target_include_directories(aoc12 PUBLIC ../common)
target_link_libraries(aoc12 PRIVATE ctre::ctre)

add_executable(aoc12ex aoc12ex.cpp)
target_include_directories(aoc12ex PUBLIC ../common)
target_link_libraries(aoc12ex PRIVATE ctre::ctre)

add_executable(aoc12o aoc12o.cpp)
target_include_directories(aoc12o PUBLIC ../common)
target_link_libraries(aoc12o PRIVATE ctre::ctre)
