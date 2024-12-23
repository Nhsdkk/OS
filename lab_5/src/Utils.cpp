//
// Created by Anton on 12/21/2024.
//

#include "Utils.h"

size_t Utils::startProcess(const std::string &childPath, const size_t id, const std::string& inputTopic, const std::string& outputTopic) {
    pid_t pid = fork();

    if (pid == ERROR){
        perror("Can't fork process");
        exit(ERROR);
    }

    if (pid == CHILD_PROCESS){
        if (execl(childPath.c_str(), std::to_string(id).c_str(), inputTopic.c_str(), outputTopic.c_str(), nullptr) == ERROR){
            std::cout << "Something went wrong when creating process " << childPath << std::endl;
        }
    }

    return pid;
}
std::vector<std::string> Utils::split(const std::string &data, char splitter) {
    std::istringstream iss(data);
    std::string buffer;
    std::vector<std::string> result;
    while (getline(iss, buffer, splitter)){
        result.push_back(buffer);
    }
    return result;
}
std::shared_ptr<RdKafka::Conf> Utils::createConfig(const std::string &broker, const std::string& groupId) {
    auto config = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    std::string errstr;
    if (config->set("bootstrap.servers", broker, errstr)){
        throw std::invalid_argument("Can't set servers: " + errstr);
    }
    if (config->set("auto.offset.reset", "earliest", errstr)){
        throw std::invalid_argument("Can't set servers: " + errstr);
    }
    if (config->set("group.id", "__consumer_offsets", errstr)){
        throw std::invalid_argument("Can't set group.id: " + errstr);
    }
    if (config->set("security.protocol", "SASL_PLAINTEXT", errstr)){
        throw std::invalid_argument("Can't set group.id: " + errstr);
    }
    if (config->set("sasl.mechanism", "PLAIN", errstr)){
        throw std::invalid_argument("Can't set group.id: " + errstr);
    }
    if (config->set("sasl.username", "admin", errstr)){
        throw std::invalid_argument("Can't set group.id: " + errstr);
    }
    if (config->set("sasl.password", "admin-secret", errstr)){
        throw std::invalid_argument("Can't set group.id: " + errstr);
    }
    return std::unique_ptr<RdKafka::Conf>(config);
}
std::shared_ptr<RdKafka::KafkaConsumer> Utils::createConsumer(const std::shared_ptr<RdKafka::Conf> &config,
                                                              const std::vector<std::shared_ptr<RdKafka::Topic>> &topics) {
    std::string errstr;
    auto consumer = RdKafka::KafkaConsumer::create(config.get(), errstr);
    if (consumer == nullptr) { throw std::invalid_argument("Can't create consumer: " + errstr);}

    std::vector<std::string> topicNames;

    for (const auto& item: topics){
        topicNames.push_back(item->name());
    }

    consumer->subscribe(topicNames);
    return std::unique_ptr<RdKafka::KafkaConsumer>(consumer);
}
std::shared_ptr<RdKafka::Producer> Utils::createProducer(const std::shared_ptr<RdKafka::Conf> &config) {
    std::string errstr;
    auto producer = RdKafka::Producer::create(config.get(), errstr);
    if (producer == nullptr) { throw std::invalid_argument("Can't create producer: " + errstr);}
    return std::unique_ptr<RdKafka::Producer>(producer);
}
std::shared_ptr<RdKafka::Topic> Utils::createTopic(const std::shared_ptr<RdKafka::Producer> &producer,
                                                   const std::string &topicName,
                                                   const std::shared_ptr<RdKafka::Conf> &config) {
    std::string errstr;
    auto topic = RdKafka::Topic::create(producer.get(), topicName, config.get(), errstr);
    if (topic == nullptr) { throw std::invalid_argument("Can't create topic: " + errstr);}
    return std::shared_ptr<RdKafka::Topic>(topic);
}
