//
// Created by Anton on 12/9/2024.
//

#include <iostream>
#include "Worker.h"

constexpr int ERROR = -1;
constexpr int CHILD_PROCESS = 0;
inline const std::string WORKER_PATH = "./worker";

size_t StartProcess(const std::string& childPath, const std::string& topic){
    pid_t pid = fork();

    if (pid == ERROR){
        perror("Can't fork process");
        exit(ERROR);
    }

    if (pid == CHILD_PROCESS){
        if (execl(childPath.c_str(), const_cast<char *>(topic.c_str()), NULL) == ERROR){
            std::cout << "Something went wrong when creating process " << childPath << std::endl;
        }
    }

    return pid == CHILD_PROCESS;
}

namespace Process {
    Worker::Worker(long id) {
        StartProcess(WORKER_PATH, std::format("node_%s", ))
    }
} // Process