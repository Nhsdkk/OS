//
// Created by nikit on 12/23/2024.
//

#ifndef OS_CP_SRC_BENCHMARK_H_
#define OS_CP_SRC_BENCHMARK_H_

#include <type_traits>
#include <cstddef>
#include "BaseMemoryResource.h"
namespace Benchmark{
    template<class T>
    requires std::is_default_constructible_v<T>
    size_t benchmark_allocator(size_t iterations, Memory::BaseMemoryResource* memory_resource){
        std::pmr::polymorphic_allocator<T> allocator(memory_resource);

        std::vector<T*> objects;
        for (auto i = 0; i < iterations; ++i){
            objects.push_back(new (allocator.allocate(1)) T());
        }

        auto usedMem = memory_resource->getTotalUsedMemory();

        for (auto obj: objects){
            allocator.deallocate(obj, 1);
        }

        return usedMem;
    }
}

#endif //OS_CP_SRC_BENCHMARK_H_
