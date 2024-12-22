//
// Created by nikit on 12/22/2024.
//

#include <chrono>
#include "Utils.h"

namespace Utils {
    double measure_time(const std::function<void()>& func) {
        auto start = std::chrono::high_resolution_clock::now();
        func();
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        return elapsed.count();
    }

    Page mallocPage(size_t pageSize) {
        auto pagePtr = std::unique_ptr<char[]>(static_cast<char*>(malloc(pageSize * sizeof (char))));
        if (pagePtr == nullptr) throw std::runtime_error("Can't allocate new page");
        return std::move(pagePtr);
    }
}
