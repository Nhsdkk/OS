//
// Created by nikit on 12/24/2024.
//

#ifndef OS_LAB_5_SRC_UTILS_H_
#define OS_LAB_5_SRC_UTILS_H_

#include <vector>
#include <string>
#include <sstream>
#include <csignal>

inline std::vector<std::string> split(const std::string &data, char splitter) {
    std::istringstream iss(data);
    std::string buffer;
    std::vector<std::string> result;
    while (getline(iss, buffer, splitter)){
        result.push_back(buffer);
    }
    return result;
}

template<class T>
inline T fromString(const std::string& data){
    std::istringstream iss(data);
    T result;
    iss >> result;
    return result;
}

inline int startProcess(const std::string &childPath, const size_t id, const int parentPortInput, const int parentPortOutput) {
    pid_t pid = fork();

    if (pid == -1){
        return -1;
    }

    if (pid == 0){
        if (execl(childPath.c_str(),  std::to_string(id).c_str(), std::to_string(parentPortInput).c_str(), std::to_string(parentPortOutput).c_str(), nullptr) == -1){
            return -1;
        }
    }

    return pid;
}

#endif //OS_LAB_5_SRC_UTILS_H_
