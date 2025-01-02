//
// Created by nikit on 12/22/2024.
//

#include "SimpleBlock.h"

namespace Memory {
    SimpleBlock &SimpleBlock::operator=(SimpleBlock &&block) noexcept {
        start = block.start;
        end = block.end;
        page = block.page;
        sz = block.sz;
        used = block.used;
        return *this;
    }

    bool SimpleBlock::operator==(const SimpleBlock &other) const {
        return start == other.start && end == other.end && page == other.page ;
    }

    bool SimpleBlock::operator<(const SimpleBlock &other) const {
        if (page != other.page) return page < other.page;
        return start < other.start;
    }
} // Memory