#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "../src/utils.h"
#include "../src/lib.h"

const char *const INPUT_FILE_NAME = "./input1.txt";

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

TEST(UtilsTest, ShouldRemoveAllVowelsWhenStringContainsVowels){
    std::string str = "qweyiaeiouywwwwwtplk";
    EXPECT_EQ("qwwwwwwtplk", Modify(str));
}

TEST(UtilsTest, ShouldReturnTheSameStringWhenStringDoesNotContainVowels){
    const std::string str = "qwwwwwwtplk";
    EXPECT_EQ("qwwwwwwtplk", Modify(str));
}

TEST(Should_WriteToRightFile, main_test){
    const std::array<std::string, 8> data = {
        "file1.txt\n",
        "file2.txt\n",
        "qwerty\n",
        "qqqqqqqqqqwerty\n",
        "pipe\n",
        "very long woooord\n",
        "many long words here\n",
        "\n"
    };

    const std::string expectedResult2 = "qqqqqqqqqqwrt\nvr lng wrd\nmn lng wrds hr\n";
    const std::string expectedResult1 = "qwrt\npp\n";

    FILE* iFile = fopen(INPUT_FILE_NAME, "w");

    for (const auto &item : data) {
        write(fileno(iFile), (item).c_str(), item.size());
    }

    fclose(iFile);

    std::ifstream inputFile(INPUT_FILE_NAME);

    ParentMain(inputFile);

    const std::string fileName1 = "file1.txt";
    const std::string fileName2 = "file2.txt";

    std::ifstream file1(fileName1);
    std::ifstream file2(fileName2);

    EXPECT_TRUE(file1.good());
    EXPECT_TRUE(file2.good());

    const std::string result1 = Read(file1);
    const std::string result2 = Read(file2);

    EXPECT_EQ(result1, expectedResult1);
    EXPECT_EQ(result2, expectedResult2);

    RemoveIfExists(INPUT_FILE_NAME);
    RemoveIfExists(data[0]);
    RemoveIfExists(data[1]);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}