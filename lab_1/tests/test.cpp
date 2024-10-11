#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "../src/utils.h"
#include "../src/lib.h"

const char *const INPUT_FILE_NAME = "./input.txt";

std::string Read(std::ifstream& file){
    std::string buff;
    std::string result;

    while (std::getline(file, buff)){
        result += buff + '\n';
    }

    return result;
}

void RemoveIfExists(const std::string& filename){
    if (std::filesystem::exists(filename)) std::filesystem::remove(filename);
}

TEST(Should_RemoveAllVowels_When_StringContainsVowels, utils_test){
    std::string str = "qweyiaeiouywwwwwtplk";
    ASSERT_EQ("qwwwwwwtplk", Modify(str));
}

TEST(Should_ReturnTheSameString_When_StringDoesNotContainVowels, utils_test){
    std::string str = "qwwwwwwtplk";
    ASSERT_EQ("qwwwwwwtplk", Modify(str));
}

TEST(Should_WriteToRightFile, main_test){
    std::array<std::string, 8> data = {
        "file1.txt",
        "file2.txt",
        "qwerty",
        "qqqqqqqqqqwerty",
        "pipe",
        "very long woooord",
        "many long words here",
        "\n"
    };

    std::string expectedResult2 = "qqqqqqqqqqwrt\nvr lng wrd\nmn lng wrds hr\n";
    std::string expectedResult1 = "qwrt\npp\n";

    FILE* iFile = fopen(INPUT_FILE_NAME, "w");

    for (auto &item : data) {
        write(fileno(iFile), (item + '\n').c_str(), item.size() + 1);
    }

    fclose(iFile);

    std::ifstream inputFile(INPUT_FILE_NAME);

    ParentMain(inputFile);

    std::ifstream file1(data[0]);
    std::ifstream file2(data[1]);

    ASSERT_TRUE(file1.good());
    ASSERT_TRUE(file2.good());

    std::string result1 = Read(file1);
    std::string result2 = Read(file2);

    ASSERT_EQ(result1, expectedResult1);
    ASSERT_EQ(result2, expectedResult2);

    RemoveIfExists(INPUT_FILE_NAME);
    RemoveIfExists(data[0]);
    RemoveIfExists(data[1]);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}