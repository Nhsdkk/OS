#include <gtest/gtest.h>
#include "../src/SimpleMemoryResource.h"
#include "../src/TestClass.h"
#include "../src/TwinsMemoryResource.h"

TEST(SimpleAllocatorTests, ShouldAllocateAndDeallocateCorrectly){
    auto memory_resource = new Memory::SimpleMemoryResource();
    std::pmr::polymorphic_allocator<TestInternal::TestClass> allocator(memory_resource);
    std::pmr::polymorphic_allocator<int> allocator_int(memory_resource);

    std::vector<TestInternal::TestClass*> objects;
    for (auto i = 0; i < 100; ++i){
        objects.push_back(new (allocator.allocate(1)) TestInternal::TestClass());
    }

    EXPECT_EQ(memory_resource->getTotalUsedMemory(), 100 * sizeof (TestInternal::TestClass));
    EXPECT_EQ(memory_resource->getTotalAllocatedMemory(), 2 * 4096);

    for (auto i = 0; i < objects.size() - 10; ++i){
        allocator.deallocate(objects[i], 1);
    }

    EXPECT_EQ(memory_resource->getTotalUsedMemory(), 10 * sizeof (TestInternal::TestClass));
    EXPECT_EQ(memory_resource->getTotalAllocatedMemory(), 2 * 4096);

    auto val = allocator_int.allocate(1);

    EXPECT_EQ(memory_resource->getTotalUsedMemory(), 10 * sizeof (TestInternal::TestClass) + sizeof (int));
    EXPECT_EQ(memory_resource->getTotalAllocatedMemory(), 2 * 4096);

    for (auto i = 0; i < 100 - 10; ++i){
        objects[i] = new (allocator.allocate(1)) TestInternal::TestClass();
    }

    EXPECT_EQ(memory_resource->getTotalAllocatedMemory(), 2 * 4096);

    for (auto i = 0; i < 100; ++i){
        objects.emplace_back(new (allocator.allocate(1)) TestInternal::TestClass());
    }

    EXPECT_EQ(memory_resource->getTotalUsedMemory(), 200 * sizeof (TestInternal::TestClass) + sizeof (int));
    EXPECT_EQ(memory_resource->getTotalAllocatedMemory(), 4 * 4096);

    for (auto obj: objects){
        allocator.deallocate(obj, 1);
    }

    allocator_int.deallocate(val, 1);

    EXPECT_EQ(memory_resource->getTotalUsedMemory(), 0);
    EXPECT_EQ(memory_resource->getTotalAllocatedMemory(), 4 * 4096);
}

TEST(TwinAllocatorTests, ShouldAllocateAndDeallocateCorrectly){
    auto memory_resource = new Memory::TwinsMemoryResource();
    std::pmr::polymorphic_allocator<TestInternal::TestClass> allocator(memory_resource);
    std::pmr::polymorphic_allocator<int> allocator_int(memory_resource);

    std::vector<TestInternal::TestClass*> objects;
    for (auto i = 0; i < 100; ++i){
        objects.push_back(new (allocator.allocate(1)) TestInternal::TestClass());
    }

    EXPECT_EQ(memory_resource->getTotalUsedMemory(), 100 * sizeof (TestInternal::TestClass));
    EXPECT_EQ(memory_resource->getTotalAllocatedMemory(), 4 * 4096);

    for (auto i = 0; i < objects.size() - 10; ++i){
        allocator.deallocate(objects[i], 1);
    }

    EXPECT_EQ(memory_resource->getTotalUsedMemory(), 10 * sizeof (TestInternal::TestClass));
    EXPECT_EQ(memory_resource->getTotalAllocatedMemory(), 4 * 4096);

    auto val = allocator_int.allocate(1);

    EXPECT_EQ(memory_resource->getTotalUsedMemory(), 10 * sizeof (TestInternal::TestClass) + sizeof (int));
    EXPECT_EQ(memory_resource->getTotalAllocatedMemory(), 4 * 4096);

    for (auto i = 0; i < 100 - 10; ++i){
        objects[i] = new (allocator.allocate(1)) TestInternal::TestClass();
    }

    for (auto i = 0; i < 100; ++i){
        objects.emplace_back(new (allocator.allocate(1)) TestInternal::TestClass());
    }

    EXPECT_EQ(memory_resource->getTotalUsedMemory(), 200 * sizeof (TestInternal::TestClass) + sizeof (int));
    EXPECT_EQ(memory_resource->getTotalAllocatedMemory(), 7 * 4096);

    for (auto obj: objects){
        allocator.deallocate(obj, 1);
    }

    allocator_int.deallocate(val, 1);

    EXPECT_EQ(memory_resource->getTotalUsedMemory(), 0);
    EXPECT_EQ(memory_resource->getTotalAllocatedMemory(), 7 * 4096);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}