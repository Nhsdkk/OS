//
// Created by Anton on 12/9/2024.
//

#ifndef OS_LAB_5_SRC_WORKER_H_
#define OS_LAB_5_SRC_WORKER_H_

#include <cstdio>
#include <memory>
#include "../../cmake-build-debug/_deps/librdkafka-src/src-cpp/rdkafkacpp.h"

namespace Process {

    class Worker : public std::enable_shared_from_this<Worker> {
        size_t id;
        size_t pid;
        std::shared_ptr<RdKafka::Producer> producer;
        std::shared_ptr<RdKafka::KafkaConsumer> consumer;
        std::vector<std::shared_ptr<Worker>> children;
        std::shared_ptr<RdKafka::Topic> outputTopTopic;
        std::shared_ptr<RdKafka::Topic> inputTopTopic;
        std::shared_ptr<RdKafka::Conf> config;

        std::vector<std::string> getCurrentSubscriptions();

        public:
            Worker(size_t id, size_t pid, const std::string& inputTopTopicName, const std::string& outputTopTopicName);
            bool operator==(const Worker& other) const { return id == other.id; }
            bool operator<(const Worker& other) const { return id < other.id; }
            bool operator>(const Worker& other) const { return id > other.id; }
            size_t attach(size_t nodeId, size_t pid, const std::string& inputBottomTopicName, const std::string& outputBottomTopicName);
            void detach(size_t nodeId);
            size_t getId() const { return id; }
            size_t getPid() const { return pid; }
            std::shared_ptr<RdKafka::Topic> getOutputTopTopic() const { return outputTopTopic; }
            std::shared_ptr<RdKafka::Topic> getInputTopTopic() const { return inputTopTopic; }
            std::optional<std::string> consume();
            void produceDown(const std::string& message);
            void produceUp(const std::string& message);
            ~Worker();
    };

} // Process

#endif //OS_LAB_5_SRC_WORKER_H_
