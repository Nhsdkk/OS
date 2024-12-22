//
// Created by nikit on 12/22/2024.
//

#ifndef OS_CP_SRC_TESTCLASS_H_
#define OS_CP_SRC_TESTCLASS_H_

#include <string>
#include <vector>
#include "../src/BaseMemoryResource.h"

namespace TestInternal {

    class TestClass {
        public:
            int id;
            std::pmr::string name;
            std::pmr::vector<int> data;
            size_t data_size;

            explicit TestClass(std::pmr::memory_resource* memory_resource) {
                id = 1;
                name = std::move(std::pmr::string ("HERE IS MY NAME", memory_resource));
                data = std::pmr::vector<int>({1,2,3,4,5,6,7,8}, memory_resource);
                data_size = 8;
            }


            ~TestClass() = default;
    };

} // Test

#endif //OS_CP_SRC_TESTCLASS_H_
