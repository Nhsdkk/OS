#include <iostream>
#include <fstream>
#include "src/Utils.h"
#include "src/TestClass.h"
#include "src/SimpleMemoryResource.h"
#include "src/TwinsMemoryResource.h"
#include "src/Benchmark.h"

int main(int argv, char** argc){
    auto rnd = Utils::RandomNumberGenerator();
    std::vector<Command> commands;
    const size_t commandCount = 1000000;
    const size_t baseObjectCount = 0;

    commands.reserve(commandCount);
    for (auto i = 0; i < commandCount; ++i){
        commands.push_back(rnd.generateEnum<Command>(2));
    }

    Benchmark::benchmark_allocator("simple_memory_resource.txt", new Memory::SimpleMemoryResource(), commands, baseObjectCount);
    Benchmark::benchmark_allocator("twins_memory_resource.txt", new Memory::TwinsMemoryResource(), commands, baseObjectCount);
}
