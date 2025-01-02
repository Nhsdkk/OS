#include <gtest/gtest.h>
#include "../src/Tree.h"
#include "../src/Request.h"
#include "../src/Response.h"

TEST(TreeTests, ShouldInsertCorrectly){
    std::array<size_t, 5> data = {
        4,
        2,
        3,
        5,
        6,
    };
    auto tree = Tree::Tree(data[0]);
    tree.attach(data[0], data[1]);
    tree.attach(data[0], data[2]);
    tree.attach(data[1], data[3]);
    tree.attach(data[2], data[4]);

    EXPECT_EQ(tree.getRoot()->getData(),data[0]);

    EXPECT_EQ(tree.getRoot()->getChildren().size(), 2);
    EXPECT_EQ(tree.getRoot()->getChildren()[0]->getData(), data[1]);
    EXPECT_EQ(tree.getRoot()->getChildren()[1]->getData(), data[2]);

    EXPECT_EQ(tree.getRoot()->getChildren()[0]->getChildren().size(), 1);
    EXPECT_EQ(tree.getRoot()->getChildren()[0]->getChildren()[0]->getData(), data[3]);

    EXPECT_EQ(tree.getRoot()->getChildren()[1]->getChildren().size(), 1);
    EXPECT_EQ(tree.getRoot()->getChildren()[1]->getChildren()[0]->getData(), data[4]);
}

TEST(TreeTests, ShouldRemoveCorrectly){
    std::array<size_t, 5> data = {
        4,
        2,
        3,
        5,
        6,
    };
    auto tree = Tree::Tree(data[0]);
    tree.attach(data[0], data[1]);
    tree.attach(data[0], data[2]);
    tree.attach(data[1], data[3]);
    tree.attach(data[2], data[4]);

    tree.remove(data[1]);

    EXPECT_EQ(tree.getRoot()->getData(),data[0]);
    EXPECT_EQ(tree.getRoot()->getChildren().size(), 1);

    EXPECT_EQ(tree.getRoot()->getChildren()[0]->getData(), data[2]);

    EXPECT_EQ(tree.getRoot()->getChildren()[0]->getChildren().size(), 1);
    EXPECT_EQ(tree.getRoot()->getChildren()[0]->getChildren()[0]->getData(), data[4]);
}

TEST(RequestParserTest, ShouldParseRequestCorrectlyWithoutId) {
    const std::string request = "kill 2";
    auto r = Request::Request::fromStringRequest(request, 1);
    EXPECT_EQ(r.getCommand(), "kill");
    EXPECT_EQ(r.getId(), 1);
    EXPECT_EQ(r.getArgs().size(), 0);
    EXPECT_EQ(r.getReceiver(), 2);
}

TEST(RequestParserTest, ShouldParseRequestCorrectlyWithId) {
    const std::string request = "1 create 2 3";
    auto r = Request::Request::fromStringRequest(request);
    EXPECT_EQ(r.getCommand(), "create");
    EXPECT_EQ(r.getId(), 1);
    EXPECT_EQ(r.getArgs().size(), 1);
    EXPECT_EQ(fromString<size_t>(r.getArgs()[0]), 3);
    EXPECT_EQ(r.getReceiver(), 2);
}

TEST(ResponseParserTest, ShouldParseCorrectly) {
    const std::string response = "1 create 2 3\nSUCCESS 2 CREATED SUCCESSFULLY";
    auto res = Response::Response::fromStringResponse(response);

    auto req = res.getRequest();

    EXPECT_EQ(req.getCommand(), "create");
    EXPECT_EQ(req.getId(), 1);
    EXPECT_EQ(req.getArgs().size(), 1);
    EXPECT_EQ(fromString<size_t>(req.getArgs()[0]), 3);
    EXPECT_EQ(req.getReceiver(), 2);

    EXPECT_EQ(res.getType(), Response::ResponseType::SUCCESS);
    EXPECT_EQ(res.getMessage(), "CREATED SUCCESSFULLY");
    EXPECT_EQ(res.getHandlerId(), 2);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}