cmake_minimum_required(VERSION 3.10)

set(SubProjectName "lab_5")

# Загрузка и сборка ZeroMQ
FetchContent_Declare(
        zeromq
        GIT_REPOSITORY https://github.com/zeromq/libzmq.git
        GIT_TAG        v4.3.5  # Укажите нужную версию
)

FetchContent_MakeAvailable(zeromq)

# Загрузка и сборка cppzmq
FetchContent_Declare(
        cppzmq
        GIT_REPOSITORY https://github.com/zeromq/cppzmq.git
        GIT_TAG        v4.8.0  # Укажите нужную версию
)

FetchContent_MakeAvailable(cppzmq)

add_library(src
    src/Worker.cpp
    src/Worker.h
    src/Utils.cpp
    src/Utils.h
    src/Response.cpp
    src/Response.h
    src/Request.cpp
    src/Request.h
    src/constants.h
    src/net.cpp
    src/net.h
        src/Tree.cpp
        src/Tree.h
)

target_link_libraries(src PRIVATE cppzmq)

add_executable(child
    child.cpp
)

target_link_libraries(child PRIVATE src cppzmq)

add_executable(lab_5 main.cpp)
target_link_libraries(lab_5 PRIVATE src cppzmq)

add_dependencies(lab_5 child)

add_executable(lab_5_tests tests/test.cpp)
target_link_libraries(lab_5_tests PRIVATE src gtest_main)

enable_testing()
add_test(NAME lab_5_tests COMMAND LW5_test)