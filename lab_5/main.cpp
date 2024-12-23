#include <iostream>
#include <map>
#include <thread>
#include "src/MasterNode.h"
#include "src/Utils.h"

inline const std::string WORKER_PATH = "./child";

int main(int argv, char** argc){
    auto masterNode = Process::MasterNode();
    bool stop = false;
    std::thread consumerThread([&masterNode, &stop]() {
        while (!stop){
            auto msg = masterNode.consume();
            if (!msg.has_value()) continue;
            std::cout << "Got message from children: " << msg.value() << std::endl;
        }
    });

    std::string message;
    std::cout << "Enter message: " << std::endl;
    while (getline(std::cin, message)) {
        if (message.find("create -1") != std::string::npos){
            std::stringstream stringstream(Utils::split(message, ' ')[2]);
            size_t nodeId;
            stringstream >> nodeId;

            std::stringstream inputBottomTopicName, outputBottomTopicName;
            inputBottomTopicName << "node_" << masterNode.getId() << "_" << nodeId;
            outputBottomTopicName << "node_" << nodeId  << "_" << masterNode.getId();

            auto pid = Utils::startProcess(WORKER_PATH, nodeId, inputBottomTopicName.str(), outputBottomTopicName.str());
            if (pid == 0) return 0;
            masterNode.attach(nodeId, pid, inputBottomTopicName.str(), outputBottomTopicName.str());
            std::cout << "Enter message: " << std::endl;
            continue;
        }

        masterNode.produceDown("INPUT " + message);

        std::cout << "Enter message: " << std::endl;
    }

    consumerThread.join();
}
