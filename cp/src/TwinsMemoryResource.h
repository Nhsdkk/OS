//
// Created by nikit on 12/22/2024.
//

#ifndef OS_CP_SRC_TWINSMEMORYRESOURCE_H_
#define OS_CP_SRC_TWINSMEMORYRESOURCE_H_

#include <map>
#include <list>
#include "BaseMemoryResource.h"
#include "cmath"
#include "TwinBlock.h"

size_t getBlockSize(size_t requestedSize) { return static_cast<size_t>(std::pow(2, std::ceil(std::log2(requestedSize)))); }

std::optional<std::string> tryFindFreeBlock(const std::map<std::string, Memory::TwinBlock>& blocks) {
    for (const auto& [key, block] : blocks){
        if (!block.getIsUsed()) return key;
    }
    return std::nullopt;
}

std::string recSplit(
    std::map<size_t, std::map<std::string, Memory::TwinBlock>>& blocks,
    size_t curSize,
    std::string idxToSplit,
    size_t targetSize
) {
    while (curSize != targetSize){
        auto [block1, block2]  = blocks.at(curSize).at(idxToSplit).split().value();
        idxToSplit = block1.getCurrentIdx();
        blocks[curSize / 2].insert({block1.getCurrentIdx(), block1});
        blocks[curSize / 2].insert({block2.getCurrentIdx(), block2});
        curSize /= 2;
    }

    return idxToSplit;
}

std::optional<std::string> tryFindBlockPostSplit(
    std::map<size_t, std::map<std::string, Memory::TwinBlock>>& blocks,
    size_t targetSize,
    size_t maxSize
) {
    size_t size = targetSize * 2;
    std::optional<std::string> idx = std::nullopt;
    while (size <= maxSize){
        auto foundIdx = tryFindFreeBlock(blocks.at(size));
        if (foundIdx.has_value()) {
            idx = foundIdx.value();
            break;
        }
        size *= 2;
    }

    if (!idx.has_value()) return std::nullopt;

    return recSplit(blocks, size, idx.value(), targetSize);
}

std::string addPageAndGetBlock(
    std::vector<Utils::Page>& pages,
    std::map<size_t, std::map<std::string, Memory::TwinBlock>>& blocks,
    size_t targetSize,
    size_t pageSize
) {
    size_t newPageIdx = pages.size();
    pages.push_back(Utils::mallocPage(pageSize));
    auto block = Memory::TwinBlock(0, pageSize, newPageIdx);
    blocks[pageSize].insert({block.getCurrentIdx(), block});

    return recSplit(blocks, pageSize, block.getCurrentIdx(), targetSize);
}


std::string findBlock(
    std::vector<Utils::Page>& pages,
    std::map<size_t, std::map<std::string, Memory::TwinBlock>>& blocks,
    size_t size,
    size_t maxSize
) {
    auto existingPreSplitBlockIdx = tryFindFreeBlock(blocks.at(size));
    if (existingPreSplitBlockIdx.has_value()) return existingPreSplitBlockIdx.value();
    auto existingPostSplitBlockIdx = tryFindBlockPostSplit(blocks, size, maxSize);
    if (existingPostSplitBlockIdx.has_value()) return existingPostSplitBlockIdx.value();
    return addPageAndGetBlock(pages, blocks, size, maxSize);
}

void reCouple(
    std::map<size_t, std::map<std::string, Memory::TwinBlock>>& blocks,
    std::string blockToFreeIdx,
    size_t size,
    size_t maxSize
) {
    blocks.at(size).at(blockToFreeIdx).free();
    while (size <= maxSize) {
        auto curIdx = blocks.at(size).at(blockToFreeIdx).getCurrentIdx();
        auto twinIdx = blocks.at(size).at(blockToFreeIdx).getTwinIdx();
        if (!twinIdx.has_value() || blocks.at(size).at(twinIdx.value()).getIsUsed()) break;
        blockToFreeIdx = blocks.at(size).at(blockToFreeIdx).getParentIdx().value();
        blocks.at(size).erase(curIdx);
        blocks.at(size).erase(twinIdx.value());
        size *= 2;
        blocks.at(size).at(blockToFreeIdx).free();
    }
}

namespace Memory {

    template<size_t basePageSize = 4096>
    class TwinsMemoryResource : public BaseMemoryResource {
        std::map<size_t, std::map<std::string, TwinBlock>> blocks;
        public:
            TwinsMemoryResource() : blocks(), BaseMemoryResource() {
                for (size_t size = basePageSize; size >= 1; size /= 2){
                    blocks.insert({size, {}});
                }
            }

            void * do_allocate(std::size_t bytes, std::size_t alignment) override {
                auto sz = getBlockSize(bytes);
                if (sz > basePageSize) throw std::runtime_error("Size exceeds base page size");

                auto idx = findBlock(pages, blocks, sz, basePageSize);
                auto [start, page] = blocks.at(sz).at(idx).use();
                setTotalUsedMemory(getTotalUsedMemory() + bytes);
                return pages[page].get() + start;
            }

            void do_deallocate(void *p, std::size_t bytes, std::size_t alignment) override {
                auto sz = getBlockSize(bytes);

                std::optional<std::string> idx;
                for (auto& [key, block] : blocks.at(sz)){
                    if (!block.getIsUsed()) continue;
                    if (p == pages[block.getPage()].get() + block.getStart()){
                        idx = key;
                        break;
                    }
                }

                if (!idx.has_value()) throw std::runtime_error("Can't find block to free");

                reCouple(blocks, idx.value(), sz, basePageSize);
            }

            bool do_is_equal(const std::pmr::memory_resource &other) const noexcept override {
                return this == &other;
            }

            ~TwinsMemoryResource() override = default;
    };

} // Memory

#endif //OS_CP_SRC_TWINSMEMORYRESOURCE_H_
