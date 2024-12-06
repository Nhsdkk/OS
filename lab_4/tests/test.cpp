#include <gtest/gtest.h>
#include <random>
#include "../src/f1.h"
#include "../src/f2.h"

constexpr int Size = 10;

std::vector<int> generateVector(const size_t vecSize){
    std::vector<int> vec(vecSize);

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<> rand(0,100);


    for (auto i = 0; i < vecSize; ++i){
        vec[i] = rand(rng);
    }

    return vec;
}

TEST(BubbleSortTests, ShouldSortCorrectly){
    auto vec1 = generateVector(Size);

    auto result = SortBubble(vec1);

    std::sort(vec1.begin(), vec1.end());
    EXPECT_EQ(result, vec1);
}

TEST(HoarSortTests, ShouldSortCorrectly){
    auto vec1 = generateVector(Size);

    auto res = SortHoar(vec1);

    std::sort(vec1.begin(), vec1.end());
    EXPECT_EQ(res, vec1);
}

TEST(PiTests, ShouldCalculatePiCorrectlyWithValles){
    EXPECT_NEAR(std::numbers::pi, CalcPiVallis(1e6), 3e-4);
}

TEST(PiTests, ShouldCalculatePiCorrectlyWithLeibniz){
    EXPECT_NEAR(std::numbers::pi, CalcPiLeibniz(1e6), 1e-5);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}