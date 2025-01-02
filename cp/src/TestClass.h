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
            std::string name;
            std::vector<int> data;
            size_t data_size;

            TestClass() {
                id = 1;
                name = "HERE IS MY NAME";
                data = std::vector<int>({1,2,3,4,5,6,7,8});
                data_size = 8;
            }


            ~TestClass() = default;
    };

} // Test

#endif //OS_CP_SRC_TESTCLASS_H_
