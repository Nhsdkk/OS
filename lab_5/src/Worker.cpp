//
// Created by Anton on 12/9/2024.
//

#include <iostream>
#include <sstream>
#include <optional>
#include <fcntl.h>
#include <semaphore>
#include "Worker.h"
#include "Utils.h"

inline const std::string BROKER = "localhost:9093";
constexpr int partition = 1;

namespace Process {
    size_t Worker::attach(size_t nodeId, size_t pid, const std::string& inputBottomTopicName, const std::string& outputBottomTopicName) {
        auto worker = std::make_shared<Worker>(nodeId, pid, inputBottomTopicName, outputBottomTopicName);
        children.push_back(worker);
        auto subs = getCurrentSubscriptions();
        subs.push_back(worker->getOutputTopTopic()->name());
        consumer->subscribe(subs);
        return worker->getPid();
    }

    void Worker::detach(size_t nodeId) {
        size_t idx = 0;
        bool found = false;
        for (const auto& child: children){
            if (child->getId() == nodeId){
                found = true;
                break;
            }
            idx++;
        }

        if (!found) throw std::invalid_argument("Can't find node");

        children.erase(
            std::remove_if(
                children.begin(),
                children.end(),
                [nodeId](auto item) { return item->getId() == nodeId; }
            ),
            children.end()
        );
    }

    Worker::~Worker() {
        for (auto& child: children){
            std::string message = "kill " + std::to_string(child->id);
            producer->produce(
                child->inputTopTopic.get(),
                RdKafka::Topic::PARTITION_UA,
                RdKafka::Producer::RK_MSG_COPY,
                (void *)message.c_str(),
                sizeof (message.c_str()),
                nullptr,
                nullptr
            );
            producer->flush(1000);
        }
    }

    std::optional<std::string> Worker::consume() {
        auto msg = std::shared_ptr<RdKafka::Message>(consumer->consume(1000));
        if (msg->err() != RdKafka::ERR_NO_ERROR) {
            return std::nullopt;
        }

        auto payload = static_cast<char *>(msg->payload());
        if (payload == nullptr) return std::nullopt;
        return std::string(payload);
    }

    void Worker::produceDown(const std::string &message) {
        for (auto& child: children){
            producer->produce(
                child->inputTopTopic.get(),
                RdKafka::Topic::PARTITION_UA,
                RdKafka::Producer::RK_MSG_COPY,
                (void *)message.c_str(),
                message.size() * sizeof (char),
                nullptr,
                nullptr
            );
        }
        producer->flush(1000);
    }

    void Worker::produceUp(const std::string &message) {
        producer->produce(
            outputTopTopic.get(),
            RdKafka::Topic::PARTITION_UA,
            RdKafka::Producer::RK_MSG_COPY,
            (void *)message.c_str(),
            message.size() * sizeof (char),
            nullptr,
            nullptr
        );
    }

    std::vector<std::string> Worker::getCurrentSubscriptions() {
        std::vector<std::string> result;
        auto err = consumer->subscription(result);
        if (err != RdKafka::ERR_NO_ERROR) std::cout << err2str(err) << std::endl;
        return result;
    }

    Worker::Worker(size_t id, size_t pid, const std::string &inputTopTopicName, const std::string &outputTopTopicName) : id(id), children({}), pid(pid) {
        config = Utils::createConfig(BROKER, std::to_string(id));
        producer = Utils::createProducer(config);
        auto tconfig = std::shared_ptr<RdKafka::Conf>(RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC));
        outputTopTopic = Utils::createTopic(producer, outputTopTopicName, tconfig);
        inputTopTopic = Utils::createTopic(producer, inputTopTopicName, tconfig);

        std::vector<std::shared_ptr<RdKafka::Topic>> topicsToConsume {inputTopTopic};

        std::cout << inputTopTopic->name() << std::endl;

        consumer = Utils::createConsumer(config, topicsToConsume);
    }
} // Process