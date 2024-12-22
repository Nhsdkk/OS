//
// Created by nikit on 12/22/2024.
//

#ifndef OS_CP_SRC_BASEMEMORYRESOURCE_H_
#define OS_CP_SRC_BASEMEMORYRESOURCE_H_

#include <cstdio>
#include <memory_resource>
#include <memory>
#include "Utils.h"

namespace Memory {
    class BaseMemoryResource : public std::pmr::memory_resource {
        size_t totalAllocatedMemory;
        size_t totalUsedMemory;

        protected:
            std::vector<Utils::Page> pages;
            void setTotalUsedMemory(size_t val) { totalUsedMemory = val; }
            void setTotalAllocatedMemory(size_t val)  { totalAllocatedMemory = val; }
        public:
            BaseMemoryResource(): pages(), totalUsedMemory(0), totalAllocatedMemory(0) {}
            size_t getTotalUsedMemory() const { return totalUsedMemory; }
            size_t getTotalAllocatedMemory() const { return totalAllocatedMemory; }
            virtual void * do_allocate(std::size_t bytes, std::size_t alignment) override = 0;
            void do_deallocate(void *p, std::size_t bytes, std::size_t alignment) override = 0;
            virtual bool do_is_equal(const std::pmr::memory_resource &other) const noexcept override = 0;
            ~BaseMemoryResource() override = default;
    };

} // Memory

#endif //OS_CP_SRC_BASEMEMORYRESOURCE_H_
