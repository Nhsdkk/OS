#include <gtest/gtest.h>
#include <random>
#include "../src/timsort.h"

void generateArray(long* values, size_t size) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<long> dist(1, 20);

    for (auto i = 0; i < size; ++i){
        values[i] = dist(mt);
    }
}

bool validateSort(const long* values, size_t size){
    for (auto i = 0; i < size - 1; ++i){
        if (values[i] > values[i + 1]) return false;
    }

    return true;
}

bool arrayEq(const long* v1, const long* v2, size_t size){
    for (auto i = 0; i < size; ++i){
        if (v1[i] != v2[i]) return false;
    }

    return true;
}

void copyArray(const long* in, long* dest, size_t size){
    for (auto i = 0; i < size; ++i){
        dest[i] = in[i];
    }
}

TEST(SyncTests, Should_SortSyncronouslyCorrectly){
    const auto size = 1000;
    auto values = static_cast<long*>(std::malloc(sizeof(long) * size));
    generateArray(values, size);
    timsort::sort(values, size);
    EXPECT_TRUE(validateSort(values, size));
    std::free(values);
}

TEST(SyncAsyncTests, Should_ReturnSameValuesForSyncAndAsyncVersions){
    const auto size = 1000;
    auto values = static_cast<long*>(std::malloc(sizeof(long) * size));
    auto values1 = static_cast<long*>(std::malloc(sizeof(long) * size));
    generateArray(values, size);
    copyArray(values, values1, size);


    timsort::sort(values, size);
    timsort::sortAsync(values1, size, 8);

    EXPECT_TRUE(arrayEq(values, values1, size));

    std::free(values);
    std::free(values1);
}

TEST(SyncAsyncTests, Should_SortSyncronouslyFaster){
    const auto size = 100000;
    auto values = static_cast<long*>(std::malloc(sizeof(long) * size));
    auto values1 = static_cast<long*>(std::malloc(sizeof(long) * size));
    generateArray(values, size);
    copyArray(values, values1, size);

    const auto startSync{std::chrono::high_resolution_clock::now()};
    timsort::sort(values, size);
    const auto endSync{std::chrono::high_resolution_clock::now()};

    auto sync = std::chrono::duration_cast<std::chrono::microseconds>(endSync - startSync);

    const auto startAsync{std::chrono::high_resolution_clock::now()};
    timsort::sortAsync(values1, size, 8);
    const auto endAsync{std::chrono::high_resolution_clock::now()};

    auto async = std::chrono::duration_cast<std::chrono::microseconds>(endAsync - startAsync);

    EXPECT_TRUE(arrayEq(values, values1, size));
    EXPECT_TRUE(async < sync);

    std::free(values);
    std::free(values1);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}