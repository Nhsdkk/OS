cmake_minimum_required(VERSION 3.10)

set(SubProjectName "lab_3")

add_executable(lab_3 main.cpp)

add_executable(child1 src/child1.cpp)

add_executable(child2 src/child2.cpp)

add_library(utils src/utils.cpp src/utils.h src/SharedObjectHandler.cpp)
add_library(constants src/constants.h)
add_library(src src/lib.h src/lib.cpp)

set_target_properties(constants PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(src PRIVATE utils constants)
target_link_libraries(lab_3 PRIVATE utils constants src)
target_link_libraries(child1 PRIVATE utils constants)
target_link_libraries(child2 PRIVATE utils constants)

add_dependencies(lab_3 child1 child2)

add_executable(lab_3_tests tests/test.cpp)
add_dependencies(lab_3_tests child1 child2)
target_link_libraries(lab_3_tests PRIVATE src utils gtest_main)

enable_testing()
add_test(NAME lab_3_tests COMMAND LW1_test)