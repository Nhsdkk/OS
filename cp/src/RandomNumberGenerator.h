//
// Created by nikit on 12/27/2024.
//

#ifndef OS_CP_SRC_RANDOMNUMBERGENERATOR_H_
#define OS_CP_SRC_RANDOMNUMBERGENERATOR_H_

#include <random>
namespace Utils {

class RandomNumberGenerator {
    std::mt19937 rng;
 public:
    RandomNumberGenerator() : rng() {}
    explicit RandomNumberGenerator(std::random_device& rd) : rng(rd()) {}

    int generateInt(int min, int max);
    template<class T>
    T generateEnum(size_t count) {
        std::uniform_int_distribution<int> distribution(0, count * 100);
        return static_cast<T>(distribution(rng) % count);
    }
};

} // Utils

#endif //OS_CP_SRC_RANDOMNUMBERGENERATOR_H_
