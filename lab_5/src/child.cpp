#include <string>
#include <sstream>
#include <optional>
#include <fstream>
#include "map"
#include "Worker.h"
#include "Utils.h"
#include "Message.h"
#include "Tree.h"

enum Result {
  OK,
  ERR
};

inline const std::string WORKER_PATH = "./child";

std::string constructMessage(const std::string& request, Result resultType, const std::string& result, size_t id){
    std::stringstream stringstream;
    switch (resultType) {
        case OK:
            stringstream << "OUTPUT" << "\n " << request << "\n" << "OK: " << id << ": " << result;
            return stringstream.str();
        case ERR:
            stringstream << "OUTPUT" << "\n " << request << "\n" << "ERROR: " << id << ": " << result;
            return stringstream.str();
    }
    return "";
}

Message::Message parseMessage(const std::string& msg) {
    auto lines = Utils::split(msg, '\n');
    auto splittedRequest = Utils::split(lines[1], ' ');
    auto target = Utils::fromString<size_t>(splittedRequest[1]);
    if (lines[0] == "OUTPUT") {
        return {Message::MessageType::OUTPUT, splittedRequest, Utils::split(lines[2], ' '), target};
    }

    return {Message::MessageType::INPUT, splittedRequest, std::nullopt, target};
}

bool checkIfMessageIsForMe(const std::string& msg, size_t recipientId, std::shared_ptr<Process::Worker>& worker){
    if (worker->getId() != recipientId) {
        worker->produceDown(msg);
        return false;
    }

    return true;
}

void handleAttach(const std::string& request, std::shared_ptr<Process::Worker> &worker, const std::string& newId) {
    std::istringstream iss(newId);
    size_t id;
    iss >> id;

    std::stringstream inputBottomTopicName, outputBottomTopicName;
    inputBottomTopicName << "node_" << worker->getId() << "_" << id;
    outputBottomTopicName << "node_" << id  << "_" << worker->getId();

    auto pid = Utils::startProcess(WORKER_PATH, id, inputBottomTopicName.str(), outputBottomTopicName.str());
    if (pid == 0) return;
    worker->attach(id, pid, inputBottomTopicName.str(), outputBottomTopicName.str());
    std::cout << "Attaching..." << std::endl;

    auto result = constructMessage(request, OK, std::to_string(pid), worker->getId());
    worker->produceUp(result);
}

void handlePing(const std::string& request, std::shared_ptr<Process::Worker> &worker) {
    auto result = constructMessage(request, OK, std::to_string(true), worker->getId());
    std::cout << "handling ping" << std::endl;
    worker->produceUp(result);
}

void handleExec(
    const std::string& request,
    std::map<std::string, int>& state,
    const std::string& key,
    const std::optional<std::string>& val,
    std::shared_ptr<Process::Worker>& worker
){
    if (val.has_value()){
        std::istringstream iss(val.value());
        int valInt;
        iss >> valInt;

        if (state.find(key) != state.end()) state[key] = valInt;
        else state.insert({key, valInt});

        worker->produceUp(constructMessage(request, OK, std::to_string(valInt), worker->getId()));
        return;
    }

    if (state.find(key) != state.end()) worker->produceUp(constructMessage(request, OK, std::to_string(state[key]), worker->getId()));
    else worker->produceUp(constructMessage(request, ERR, "Can't find key " + key, worker->getId()));
}

int main(int argc, char ** argv){
    if (argc != 3) return -1;
    std::stringstream strsteam(argv[0]);
    size_t id;
    strsteam >> id;
    std::string inputTopic(argv[1]);
    std::string outputTopic(argv[2]);

    auto worker = std::make_shared<Process::Worker>(id, getpid(), inputTopic, outputTopic);
    std::map<std::string, int> state;
    Tree::Tree<size_t> tree(id);

    while (true){
        std::optional<std::string> message = worker->consume();
        if (!message.has_value()) continue;

        auto msg = parseMessage(message.value());

        if (msg.getType() == Message::OUTPUT){
            if (msg.getCommandTextSplitted()[0] == "kill") {
                auto targetToKill = Utils::fromString<size_t>(msg.getCommandTextSplitted()[1]);
                tree.remove(targetToKill);
            }else if (msg.getCommandTextSplitted()[0] == "attach") {
                auto targetToAttach = Utils::fromString<size_t>(msg.getCommandTextSplitted()[1]);
                auto nodeToAttach = Utils::fromString<size_t>(msg.getCommandTextSplitted()[2]);
                tree.attach(targetToAttach, nodeToAttach);
            }

            worker->produceUp(message.value());
        }

        auto txt = msg.getCommandTextSplitted();
        auto workerId = Utils::fromString<size_t>(txt[1]);
        auto command = txt[0];
        auto args = std::vector(txt.begin() + 3, txt.end());

        const auto& commandText = msg.getCommandText();

        if (command == "kill"){
            if (checkIfMessageIsForMe(message.value(), workerId, worker)) {
                worker->produceUp(constructMessage(commandText, OK, "Killing worker", worker->getId()));
                break;
            }
            continue;
        }

        if (command == "ping"){
            if (!checkIfMessageIsForMe(message.value(), workerId, worker)) continue;
            handlePing(commandText, worker);
            continue;
        }

        if (command == "attach"){
            if (!checkIfMessageIsForMe(message.value(), workerId, worker)) continue;
            if (args.size() != 1) {
                worker->produceUp(constructMessage(commandText, ERR, "Message is malformed", worker->getId()));
                continue;
            }
            handleAttach(commandText, worker, args[0]);
            continue;
        }

        if (command == "exec"){
            if (!checkIfMessageIsForMe(message.value(), workerId, worker)) continue;
            if (args.empty()) {
                worker->produceUp(constructMessage(commandText, ERR, "Message is malformed", worker->getId()));
                continue;
            }
            std::optional<std::string> val = std::nullopt;
            if (args.size() == 2) val = args[1];

            handleExec(commandText, state, args[0], val, worker);
            continue;
        }

    }
}