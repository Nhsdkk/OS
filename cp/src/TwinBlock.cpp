//
// Created by nikit on 12/22/2024.
//

#include <stdexcept>
#include "TwinBlock.h"

namespace Memory {
    std::optional<std::tuple<TwinBlock, TwinBlock>> TwinBlock::split() {
        if (reservedSize == 1 || used) return std::nullopt;
        used = true;
        auto block1Idx = constructIdx(page, start, start + reservedSize / 2 - 1);
        auto block2Idx = constructIdx(page, start + reservedSize / 2, end);
        auto block1 = TwinBlock(start, start + reservedSize / 2 - 1, page, block2Idx, currentIdx, false);
        auto block2 = TwinBlock(start + reservedSize / 2, end, page,  block1Idx, currentIdx, false);
        return std::tuple<TwinBlock, TwinBlock>(block1, block2);
    }

    void TwinBlock::free() {
        if (!used) throw std::runtime_error("Can't free unused block");
        used = false;
    }

    std::tuple<size_t, size_t> TwinBlock::use() {
        if (used) throw std::runtime_error("Can't use already used block");
        used = true;
        return {start, page};
    }
} // Memory