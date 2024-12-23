//
// Created by nikit on 12/22/2024.
//

#ifndef OS_CP_SRC_SIMPLEMEMORYRESOURCE_H_
#define OS_CP_SRC_SIMPLEMEMORYRESOURCE_H_

#include <cstdio>
#include <memory_resource>
#include <memory>
#include <list>
#include <algorithm>
#include <iostream>
#include "SimpleBlock.h"
#include "BaseMemoryResource.h"

std::tuple<Memory::SimpleBlock, size_t> findPlaceToInsert(const size_t sz, const std::vector<Memory::SimpleBlock>& blocks){
    size_t idx = 0;
    for (auto& block: blocks) {
        if (block.get_used()) {
            idx++;
            continue;
        }

        if (block.get_size() >= sz){
            return {{block.get_start(), block.get_start() + sz - 1, block.get_page(), true}, idx};
        }

        idx++;
    }
    int lastPage = -1;
    if (!blocks.empty()) lastPage = blocks.back().get_page();
    return {{0, sz - 1, static_cast<size_t>(lastPage + 1), true}, blocks.size()};
}

namespace Memory {

    template<size_t basePageSize = 4096>
    class SimpleMemoryResource : public BaseMemoryResource {
        std::vector<SimpleBlock> blocks;
        public:
            SimpleMemoryResource() : blocks(), BaseMemoryResource() {}

            void * do_allocate(std::size_t bytes, std::size_t alignment) override {
                auto [block, emplaceBlockIdx] = findPlaceToInsert(bytes, blocks);

                if (emplaceBlockIdx == blocks.size()){
                    pages.push_back(Utils::mallocPage(basePageSize));
                    blocks.push_back(block);
                    if (basePageSize != bytes) blocks.emplace_back(block.get_end() + 1, basePageSize - 1, block.get_page(), false);
                    std::sort(blocks.begin(), blocks.end());
                    setTotalUsedMemory(getTotalUsedMemory() + bytes);
                    setTotalAllocatedMemory(getTotalAllocatedMemory() + basePageSize);
                    return pages[block.get_page()].get() + block.get_start();
                }

                if (blocks[emplaceBlockIdx].get_size() == bytes) {
                    blocks[emplaceBlockIdx].set_used(true);
                    setTotalUsedMemory(getTotalUsedMemory() + bytes);
                    return pages[block.get_page()].get() + block.get_start();
                }

                blocks[emplaceBlockIdx].set_start(block.get_end() + 1);
                blocks.push_back(block);
                std::sort(blocks.begin(), blocks.end());
                setTotalUsedMemory(getTotalUsedMemory() + bytes);
                return pages[block.get_page()].get() + block.get_start();
            }

            void do_deallocate(void *p, std::size_t bytes, std::size_t alignment) override {
                auto idx = 0;
                for (auto& block : blocks){
                    if (!block.get_used()) {
                        idx++;
                        continue;
                    }
                    if (p == pages[block.get_page()].get() + block.get_start()){
                        break;
                    }
                    idx++;
                }

                if (idx == blocks.size() || !blocks[idx].get_used()) throw std::logic_error("Can't free unused memory");
                blocks[idx].set_used(false);

                if (idx != 0 && blocks[idx].get_start() != 0 && !blocks[idx - 1].get_used()) {
                    auto start = blocks[idx - 1].get_start();
                    blocks[idx].set_start(start);
                    blocks.erase(std::remove(blocks.begin(), blocks.end(), blocks[idx - 1]), blocks.end());
                }

                if (idx != blocks.size() - 1 && blocks[idx].get_end() != basePageSize - 1 && !blocks[idx + 1].get_used()) {
                    auto end = blocks[idx + 1].get_end();
                    blocks[idx].set_end(end);
                    blocks.erase(std::remove(blocks.begin(), blocks.end(), blocks[idx + 1]), blocks.end());
                }

                setTotalUsedMemory(getTotalUsedMemory() - bytes);
                std::sort(blocks.begin(), blocks.end());
            }

            bool do_is_equal(const std::pmr::memory_resource &other) const noexcept override {
                return this == &other;
            }

            ~SimpleMemoryResource() override = default;
    };

} // Memory

#endif //OS_CP_SRC_SIMPLEMEMORYRESOURCE_H_
