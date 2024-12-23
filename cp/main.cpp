#include <iostream>
#include <fstream>
#include "src/Utils.h"
#include "src/TestClass.h"
#include "src/SimpleMemoryResource.h"
#include "src/TwinsMemoryResource.h"
#include "src/Benchmark.h"

void benchmark(size_t iterations){
    std::cout << "Running benchmark for " << iterations << " iterations" << std::endl;
    auto simpleMemRes = new Memory::SimpleMemoryResource();
    auto twinsMemRes = new Memory::TwinsMemoryResource();

    std::ofstream file;
    file.open("data.txt", std::ios_base::app);

    auto [timeSimple, usedMemSimple] = Utils::measure_time([&simpleMemRes, iterations](){
      return Benchmark::benchmark_allocator<TestInternal::TestClass>(iterations, simpleMemRes);
    });

    auto [timeTwin, usedMemTwin] = Utils::measure_time([&twinsMemRes, iterations](){
      return Benchmark::benchmark_allocator<TestInternal::TestClass>(iterations, twinsMemRes);
    });

    file << iterations << " ";
    file << timeSimple << " " << simpleMemRes->getTotalAllocatedMemory() << " " << usedMemSimple << " ";
    file << timeTwin << " " << twinsMemRes->getTotalAllocatedMemory() << " " << usedMemTwin;
    file << std::endl;
}

int main(int argv, char** argc){
    {
        std::ofstream file;
        file.open("data.txt", std::ios_base::app);
        file << "allocations timeSimple allocatedMemorySimple usedMemSimple timeTwin allocatedMemoryTwin usedMemTwin" << std::endl;
    }
    for (auto i = 10; i < 10000; i+=10){
        benchmark(i);
    }
}
