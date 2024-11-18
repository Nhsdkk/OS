cmake_minimum_required(VERSION 3.10)

set(SubProjectName "lab_2")

add_executable(lab_2 main.cpp)

add_library(timsort src/timsort.cpp src/timsort.h)

target_link_libraries(lab_2 PRIVATE timsort)

add_executable(lab_2_tests tests/test.cpp)
target_link_libraries(lab_2_tests PRIVATE timsort gtest_main)

enable_testing()
add_test(NAME lab_2_tests COMMAND LW2_test)