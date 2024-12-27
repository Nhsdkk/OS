//
// Created by nikit on 12/22/2024.
//

#ifndef OS_CP_SRC_UTILS_H_
#define OS_CP_SRC_UTILS_H_

#include <functional>
#include <memory>
#include <chrono>

namespace Utils {
    using Page = std::unique_ptr<char[]>;

    template<class T>
    inline std::tuple<double, T> measure_time(const std::function<T ()>& func) {
        auto start = std::chrono::high_resolution_clock::now();
        auto res = func();
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        return {elapsed.count(), res};
    }

    Page mallocPage(size_t pageSize);
}

#endif //OS_CP_SRC_UTILS_H_
