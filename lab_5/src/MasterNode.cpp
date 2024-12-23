//
// Created by Anton on 12/21/2024.
//

#include <iostream>
#include "MasterNode.h"
#include "Utils.h"

inline const std::string BROKER = "localhost:9093";
inline const std::string WORKER_PATH = "./child";

namespace Process {
    size_t MasterNode::attach(size_t nodeId, size_t pid, const std::string& inputBottomTopicName, const std::string& outputBottomTopicName) {
        auto worker = std::make_shared<Worker>(nodeId, pid, inputBottomTopicName, outputBottomTopicName);
        children.push_back(worker);
        auto subs = getCurrentSubscriptions();
        subs.push_back(worker->getOutputTopTopic()->name());
        consumer->subscribe(subs);
        return worker->getPid();
    }

    MasterNode::MasterNode() : children({}) {
        config = Utils::createConfig(BROKER, std::to_string(id));
        producer = Utils::createProducer(config);
        consumer = Utils::createConsumer(config, {});
    }

    std::vector<std::string> MasterNode::getCurrentSubscriptions() {
        std::vector<std::string> result;
        auto err = consumer->subscription(result);
        if (err != RdKafka::ERR_NO_ERROR) std::cout << err2str(err) << std::endl;
        return result;
    }

    std::optional<std::string> MasterNode::consume() {
        auto msg = std::shared_ptr<RdKafka::Message>(consumer->consume(1000));
        if (msg->err() == RdKafka::ERR_NO_ERROR) return std::string(static_cast<char*>(msg->payload()));
//        std::cout << msg->errstr() << std::endl;
        return std::nullopt;
    }

    void MasterNode::produceDown(const std::string &message) {
        for (auto& child: children){
            producer->produce(
                child->getInputTopTopic().get(),
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
} // Process