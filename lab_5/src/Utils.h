#pragma once
#include <string>
#include <csignal>
#include <iostream>
#include <memory>
#include <sstream>
#include "../../cmake-build-debug/_deps/librdkafka-src/src-cpp/rdkafkacpp.h"

constexpr int ERROR = -1;
constexpr int CHILD_PROCESS = 0;

namespace Utils{

    std::vector<std::string> split(const std::string& data, char splitter);

    size_t startProcess(const std::string &childPath, size_t id, const std::string& inputTopic, const std::string& outputTopic);

    std::shared_ptr<RdKafka::Conf> createConfig(const std::string& broker, const std::string& groupId);

    std::shared_ptr<RdKafka::KafkaConsumer> createConsumer(const std::shared_ptr<RdKafka::Conf>& config, const std::vector<std::shared_ptr<RdKafka::Topic>>& topics);

    std::shared_ptr<RdKafka::Producer> createProducer(const std::shared_ptr<RdKafka::Conf>& config);

    std::shared_ptr<RdKafka::Topic> createTopic(
        const std::shared_ptr<RdKafka::Producer>& producer,
        const std::string& topicName,
        const std::shared_ptr<RdKafka::Conf>& config
    );

    template<class T>
    T fromString(const std::string& data){
        std::istringstream iss(data);
        T result;
        iss >> result;
        return result;
    }

}
