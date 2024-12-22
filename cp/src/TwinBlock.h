//
// Created by nikit on 12/22/2024.
//

#ifndef OS_CP_SRC_TWINBLOCK_H_
#define OS_CP_SRC_TWINBLOCK_H_

#include <cstdio>
#include <optional>
#include <tuple>

static inline std::string constructIdx(size_t page, size_t start, size_t end) { return std::to_string(page) + std::to_string(start) + std::to_string(end); }

namespace Memory {

    class TwinBlock {
        size_t start, end, page;
        size_t reservedSize;
        std::optional<std::string> twinIdx;
        std::optional<std::string> parentIdx;
        std::string currentIdx;
        bool used;

        TwinBlock(
            size_t start,
            size_t end,
            size_t page,
            std::string twinIdx,
            std::string parentIdx,
            bool used
        ) : start(start),
            end(end),
            page(page),
            reservedSize(end - start + 1),
            twinIdx(twinIdx),
            parentIdx(parentIdx),
            used(used),
            currentIdx(constructIdx(page, start, end))
        {}

        public:
            TwinBlock(
                size_t start,
                size_t end,
                size_t page
            ) : start(start),
                end(end),
                page(page),
                reservedSize(end - start + 1),
                twinIdx(std::nullopt),
                parentIdx(std::nullopt),
                used(false),
                currentIdx(constructIdx(page, start, end))
            {}

            bool getIsUsed() const { return used; }
            size_t getStart() const { return start; }
            size_t getEnd() const { return end; }
            size_t getPage() const { return page; }
            std::optional<std::string> getTwinIdx() const { return twinIdx; }
            std::optional<std::string> getParentIdx() const { return parentIdx; }
            std::string getCurrentIdx() const { return currentIdx; }

            std::optional<std::tuple<TwinBlock, TwinBlock>> split();
            std::tuple<size_t, size_t> use();
            void free();
    };

} // Memory

#endif //OS_CP_SRC_TWINBLOCK_H_
