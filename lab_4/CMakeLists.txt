cmake_minimum_required(VERSION 3.10)

set(SubProjectName "lab_4")

add_library(f1 SHARED src/f1.h src/f1.cpp)
add_library(f2 SHARED src/f2.h src/f2.cpp)

add_executable(lab_4_static main.cpp)
target_link_libraries(lab_4_static PRIVATE f1 f2)

add_executable(lab_4_dynamic mainDynamic.cpp)

add_executable(lab_4_tests tests/test.cpp)
target_link_libraries(lab_4_tests PRIVATE f1 f2 gtest_main)

enable_testing()
add_test(NAME lab_4_tests COMMAND LW4_test)
