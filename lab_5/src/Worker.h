//
// Created by Anton on 12/9/2024.
//

#ifndef OS_LAB_5_SRC_WORKER_H_
#define OS_LAB_5_SRC_WORKER_H_

#include <cstdio>
#include <memory>
#include "../../cmake-build-debug/_deps/librdkafka-src/src-cpp/rdkafkacpp.h"

namespace Process {

    class Worker {
        long id;
        size_t pid;
        std::unique_ptr<RdKafka::Producer> producer;
        std::unique_ptr<RdKafka::Consumer> consumer;

        public:
            explicit Worker(long id);
            bool operator==(const Worker& other) const { return id == other.id; }
            bool operator<(const Worker& other) const { return id < other.id; }
            bool operator>(const Worker& other) const { return id > other.id; }
            long getId() const { return id; }
            size_t getPid() const { return pid; }
            ~Worker() = default;
    };

} // Process

#endif //OS_LAB_5_SRC_WORKER_H_
