cmake_minimum_required(VERSION 3.10)

set(SubProjectName "cp")

add_library(src
    src/SimpleMemoryResource.cpp
    src/SimpleMemoryResource.h
    src/TwinsMemoryResource.cpp
    src/TwinsMemoryResource.h
        src/SimpleBlock.cpp
        src/SimpleBlock.h
        src/BaseMemoryResource.cpp
        src/BaseMemoryResource.h
        src/Utils.cpp
        src/Utils.h
        src/TestClass.cpp
        src/TestClass.h
        src/TwinBlock.cpp
        src/TwinBlock.h
        src/Benchmark.cpp
        src/Benchmark.h
        src/RandomNumberGenerator.cpp
        src/RandomNumberGenerator.h
)

add_executable(cp main.cpp)
target_link_libraries(cp PRIVATE src)

add_executable(cp_tests tests/test.cpp)
target_link_libraries(cp_tests PRIVATE src gtest_main)

enable_testing()
add_test(NAME cp_tests COMMAND CP_test)
