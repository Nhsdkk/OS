//
// Created by nikit on 12/24/2024.
//

#ifndef OS_LAB_5_SRC_CONSTANTS_H_
#define OS_LAB_5_SRC_CONSTANTS_H_

using namespace std::chrono_literals;

const inline std::string WORKER_PATH = "./child";
constexpr int basePort = 4000;
const inline std::string baseAddress = "tcp://127.0.0.1:";
constexpr std::chrono::duration TIMEOUT = 5s;

#endif //OS_LAB_5_SRC_CONSTANTS_H_
