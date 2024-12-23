//
// Created by Anton on 12/21/2024.
//

#ifndef OS_LAB_5_SRC_MASTERNODE_H_
#define OS_LAB_5_SRC_MASTERNODE_H_

#include <optional>
#include "Worker.h"

namespace Process{

    class MasterNode {
        size_t id = 0;
        std::shared_ptr<RdKafka::Conf> config;
        std::vector<std::shared_ptr<Worker>> children;
        std::shared_ptr<RdKafka::Producer> producer;
        std::shared_ptr<RdKafka::KafkaConsumer> consumer;

        std::vector<std::string> getCurrentSubscriptions();

        public:
            MasterNode();
            std::optional<std::string> consume();
            void produceDown(const std::string& message);
            size_t attach(size_t nodeId, size_t pid, const std::string& inputBottomTopicName, const std::string& outputBottomTopicName);
            size_t getId() const { return id; }
    };

} // Process

#endif //OS_LAB_5_SRC_MASTERNODE_H_
