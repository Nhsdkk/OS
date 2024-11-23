#pragma once

#include <string>

const inline std::string CHILD_1_PATH {"./child1"};
const inline std::string CHILD_2_PATH {"./child2"};
const inline std::string PARENT_SEM_NAME {"parent"};
const inline std::string CHILD_1_OBJECT_NAME {"/child1"};
const inline std::string CHILD_2_OBJECT_NAME {"/child2"};
const constexpr int READ_END = 0;
const constexpr int WRITE_END = 1;
const constexpr int ERROR = -1;
const constexpr int MAX_STRING_LENGTH = 10;
const constexpr int CHILD_PROCESS = 0;
const constexpr size_t BUFFER_SIZE = 4096;