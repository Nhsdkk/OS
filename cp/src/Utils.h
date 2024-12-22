//
// Created by nikit on 12/22/2024.
//

#ifndef OS_CP_SRC_UTILS_H_
#define OS_CP_SRC_UTILS_H_

#include <functional>

namespace Utils {
    using Page = std::unique_ptr<char[]>;

    double measure_time(const std::function<void()>& func);
    Page mallocPage(size_t pageSize);
}

#endif //OS_CP_SRC_UTILS_H_
