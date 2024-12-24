cmake_minimum_required(VERSION 3.10)

set(SubProjectName "lab_5")

FetchContent_Declare(
        librdkafka
        GIT_REPOSITORY https://github.com/edenhill/librdkafka.git
        GIT_TAG        v2.2.0
)

FetchContent_MakeAvailable(librdkafka)

add_library(src
        src/Tree.cpp
        src/Tree.h
        src/Worker.cpp
        src/Worker.h
        src/Utils.cpp
        src/Utils.h
        src/Message.cpp
        src/Message.h
)

target_link_libraries(src PRIVATE rdkafka++)

add_library(master_node
        src/MasterNode.cpp
        src/MasterNode.h
)

target_link_libraries(master_node PRIVATE src)

add_executable(child src/child.cpp)
target_link_libraries(child PRIVATE src)

add_executable(lab_5 main.cpp)
target_link_libraries(lab_5 PRIVATE master_node)

add_dependencies(lab_5 child)

add_executable(lab_5_tests tests/test.cpp)
target_link_libraries(lab_5_tests PRIVATE src gtest_main)

enable_testing()
add_test(NAME lab_5_tests COMMAND LW5_test)