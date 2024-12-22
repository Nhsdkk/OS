//
// Created by nikit on 12/22/2024.
//

#ifndef OS_CP_SRC_SIMPLEBLOCK_H_
#define OS_CP_SRC_SIMPLEBLOCK_H_

#include <cstdio>
namespace Memory {

class SimpleBlock {
    size_t start, end, page, sz;
    bool used;

    public:
        SimpleBlock() = default;
        SimpleBlock(size_t start, size_t end, size_t page, bool used) : start(start), end(end), page(page), sz(end - start + 1), used(used) {}

        SimpleBlock(const SimpleBlock& block) = default;
        SimpleBlock(SimpleBlock&& block) noexcept = default;
        SimpleBlock& operator=(const SimpleBlock& block) = default;
        SimpleBlock& operator=(SimpleBlock&& block) noexcept;

        size_t get_start() const { return start; }
        size_t get_end() const { return end; }
        size_t get_page() const { return page; }
        size_t get_size() const { return sz; }
        size_t get_used() const { return used; }

        void set_used(bool val) { used = val; }
        void set_start(size_t val) {
            start = val;
            sz = end - start + 1;
        }
        void set_end(size_t val) {
            end = val;
            sz = end - start + 1;
        }

        ~SimpleBlock() = default;

        bool operator==(const SimpleBlock& other) const;
        bool operator<(const SimpleBlock& other) const;
};

} // Memory

#endif //OS_CP_SRC_SIMPLEBLOCK_H_
