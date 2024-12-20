cmake_minimum_required(VERSION 3.10)

set(SubProjectName "lab_5")

FetchContent_Declare(
        librdkafka
        GIT_REPOSITORY https://github.com/edenhill/librdkafka.git
        GIT_TAG        v2.2.0 # Укажите необходимую версию
)

FetchContent_MakeAvailable(librdkafka)

add_library(src
        src/Tree.cpp
        src/Tree.h
        src/Worker.cpp
        src/Worker.h
)

add_executable(child src/child.cpp)

add_executable(lab_5 main.cpp)
target_link_libraries(lab_5 PRIVATE src rdkafka)

add_executable(lab_5_tests tests/test.cpp)
target_link_libraries(lab_5_tests PRIVATE src gtest_main)

enable_testing()
add_test(NAME lab_5_tests COMMAND LW5_test)