//
// Created by nikit on 10/25/2024.
//

#include <cstdlib>
#include <random>
#include <iostream>
#include "src/timsort.h"

int main(int argc, char** argv){
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<long> dist(1, 20);

    size_t size = 24;
    auto values = static_cast<long*>(std::malloc(sizeof(long) * size));
    for (auto i = 0; i < size; ++i){
        values[i] = dist(mt);
    }

    for (auto i = 0; i < size; ++i){
        std::cout << values[i] << " ";
    }

    std::cout << std::endl;

    timsort::sortAsync(values, size);

    for (auto i = 0; i < size; ++i){
        std::cout << values[i] << " ";
    }

    std::cout << std::endl;

    std::free(values);
    return 0;
}