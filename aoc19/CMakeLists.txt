cmake_minimum_required (VERSION 3.21)

add_executable(aoc19 aoc19.cpp)
target_include_directories(aoc19 PUBLIC ../common)
target_link_libraries(aoc19 PRIVATE ctre::ctre)

#add_executable(aoc19_icl aoc19_icl.cpp)
#target_include_directories(aoc19_icl PUBLIC ../common)
#target_link_libraries(aoc19_icl PRIVATE ctre::ctre)