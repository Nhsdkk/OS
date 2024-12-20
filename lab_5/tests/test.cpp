#include <gtest/gtest.h>
#include "../src/Tree.h"
#include "../src/ProcessData.h"

TEST(BSearchTreeTests, ShouldInsertCorrectly){
    std::array<Process::ProcessData, 5> data = {
        Process::ProcessData(4),
        Process::ProcessData(2),
        Process::ProcessData(3),
        Process::ProcessData(5),
        Process::ProcessData(6),
    };
    auto tree = Tree::Tree(data[0]);
    tree.getRoot()->attachNode(data[1]);
    tree.getRoot()->attachNode(data[2]);
    tree.getRoot()->getChildren()[0]->attachNode(data[3]);
    tree.getRoot()->getChildren()[1]->attachNode(data[4]);

    EXPECT_EQ(tree.getRoot()->getData(),data[0]);

    EXPECT_EQ(tree.getRoot()->getChildren().size(), 2);
    EXPECT_EQ(tree.getRoot()->getChildren()[0]->getData(), data[1]);
    EXPECT_EQ(tree.getRoot()->getChildren()[1]->getData(), data[2]);

    EXPECT_EQ(tree.getRoot()->getChildren()[0]->getChildren().size(), 1);
    EXPECT_EQ(tree.getRoot()->getChildren()[0]->getChildren()[0]->getData(), data[3]);

    EXPECT_EQ(tree.getRoot()->getChildren()[1]->getChildren().size(), 1);
    EXPECT_EQ(tree.getRoot()->getChildren()[1]->getChildren()[0]->getData(), data[4]);
}

TEST(BSearchTreeTests, ShouldRemoveCorrectly){
    std::array<Process::ProcessData, 5> data = {
        Process::ProcessData(4),
        Process::ProcessData(2),
        Process::ProcessData(3),
        Process::ProcessData(5),
        Process::ProcessData(6),
    };
    auto tree = Tree::Tree(data[0]);
    tree.getRoot()->attachNode(data[1]);
    tree.getRoot()->attachNode(data[2]);
    tree.getRoot()->getChildren()[0]->attachNode(data[3]);
    tree.getRoot()->getChildren()[1]->attachNode(data[4]);

    tree.getRoot()->detachNode(data[1]);

    EXPECT_EQ(tree.getRoot()->getData(),data[0]);
    EXPECT_EQ(tree.getRoot()->getChildren().size(), 1);

    EXPECT_EQ(tree.getRoot()->getChildren()[0]->getData(), data[2]);

    EXPECT_EQ(tree.getRoot()->getChildren()[0]->getChildren().size(), 1);
    EXPECT_EQ(tree.getRoot()->getChildren()[0]->getChildren()[0]->getData(), data[4]);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}