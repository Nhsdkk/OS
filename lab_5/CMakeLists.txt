cmake_minimum_required(VERSION 3.10)

set(SubProjectName "lab_5")

add_library(src
        src/Tree.cpp
        src/Tree.h
        src/ProcessData.cpp
        src/ProcessData.h
)

add_executable(child src/child.cpp)

add_executable(lab_5 main.cpp)
target_link_libraries(lab_5 PRIVATE src)

add_executable(lab_5_tests tests/test.cpp)
target_link_libraries(lab_5_tests PRIVATE src gtest_main)

enable_testing()
add_test(NAME lab_5_tests COMMAND LW5_test)