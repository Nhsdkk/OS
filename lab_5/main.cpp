#include <iostream>
#include <map>
#include <thread>
#include "src/MasterNode.h"
#include "src/Utils.h"
#include "src/Message.h"
#include "src/Tree.h"

inline const std::string WORKER_PATH = "./child";

Message::Message parseResult(const std::string& msg) {
    auto lines = Utils::split(msg, '\n');
    auto splittedRequest = Utils::split(lines[1], ' ');
    auto target = Utils::fromString<size_t>(splittedRequest[1]);
    return {Message::MessageType::OUTPUT, splittedRequest, Utils::split(lines[2], ' '), target};
}

std::string constructMessage(const std::string& command){
    std::stringstream stringstream;
    stringstream << "INPUT" << "\n " << command;
    return stringstream.str();
}

Message::Message getMessageFromCommand(const std::string& command){
    auto splittedCommand = Utils::split(command, ' ');
    auto target = Utils::fromString<size_t>(splittedCommand[1]);
    return {
        Message::INPUT,
        splittedCommand,
        std::nullopt,
        target};

}

int main(int argv, char** argc){
    auto masterNode = Process::MasterNode();
    bool stop = false;
    Tree::Tree<size_t> tree(0);
    std::thread consumerThread([&tree, &masterNode, &stop]() {
        while (!stop){
            auto message = masterNode.consume();
            if (!message.has_value()) continue;
            auto msg = parseResult(message.value());
            if (msg.getCommandTextSplitted()[0] == "kill"){
                tree.remove(msg.getTargetId());
            }else if (msg.getCommandTextSplitted()[0] == "attach"){
                tree.attach(
                    msg.getTargetId(),
                    Utils::fromString<size_t>(msg.getCommandTextSplitted()[2])
                );
            }
            std::cout << "Got message from children: " << msg.getResultText() << std::endl;
        }
    });



    std::string ctxt;
    std::cout << "Enter message: " << std::endl;
    while (getline(std::cin, ctxt)) {
        auto message = getMessageFromCommand(ctxt);
        auto commandSplitted = message.getCommandTextSplitted();

        if (commandSplitted[0] == "kill" && (!tree.exists(message.getTargetId()) || message.getTargetId() == 0)) {
            std::cout << "Can't kill non-existing node" << std::endl;
            continue;
        }

        if (commandSplitted[0] == "attach" && (tree.exists(Utils::fromString<size_t>(commandSplitted[2])) || Utils::fromString<size_t>(commandSplitted[2]) == 0)){
            std::cout << "Node already exists" << std::endl;
            continue;
        }

        if (message.getTargetId() != 0) {
            masterNode.produceDown(constructMessage(ctxt));
            std::cout << "Enter message: " << std::endl;
            continue;
        }

        if (commandSplitted[0] == "attach"){
            std::stringstream stringstream(commandSplitted[2]);
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

        std::cout << "Enter message: " << std::endl;
    }

    consumerThread.join();
}
