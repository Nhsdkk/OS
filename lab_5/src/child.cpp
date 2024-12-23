#include <string>
#include <sstream>
#include <optional>
#include <fstream>
#include "map"
#include "Worker.h"
#include "Utils.h"

enum Result {
  OK,
  ERR
};

inline const std::string WORKER_PATH = "./child";

std::string constructMessage(Result resultType, const std::string& result, size_t id){
    std::stringstream stringstream;
    switch (resultType) {
        case OK:
            stringstream << "OUTPUT OK: " << id << ": " << result << std::endl;
            return stringstream.str();
        case ERR:
            stringstream << "OUTPUT ERROR: " << id << ": " << result << std::endl;
            return stringstream.str();
    }
    return "";
}

bool parseMessage(const std::string& msg, std::string& command, size_t& workerId, std::vector<std::string>& args, const std::shared_ptr<Process::Worker>& worker) {
    auto splittedMessage = Utils::split(msg, ' ');
    if (splittedMessage[0] == "OUTPUT") {
        worker->produceUp(msg);
        return true;
    }

    if (splittedMessage.size() < 3) {
        auto resultMessage = constructMessage(ERR, "Message is malformed", worker->getId());
        worker->produceUp(resultMessage);
        return true;
    }

    std::istringstream iss(splittedMessage[2]);
    iss >> workerId;

    command = splittedMessage[1];
    args = std::vector(splittedMessage.begin() + 3, splittedMessage.end());
    return false;
}

bool checkIfMessageIsForMe(const std::string& msg, size_t recipientId, std::shared_ptr<Process::Worker>& worker){
    if (worker->getId() != recipientId) {
        worker->produceDown(msg);
        return false;
    }

    return true;
}

void handleAttach(std::shared_ptr<Process::Worker> &worker, const std::string& newId) {
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

    auto result = constructMessage(OK, std::to_string(pid), worker->getId());
    worker->produceUp(result);
}

void handlePing(std::shared_ptr<Process::Worker> &worker) {
    auto result = constructMessage(OK, std::to_string(true), worker->getId());
    std::cout << "handling ping" << std::endl;
    worker->produceUp(result);
}

void handleExec(std::map<std::string, int>& state, const std::string& key, const std::optional<std::string>& val, std::shared_ptr<Process::Worker>& worker){
    if (val.has_value()){
        std::istringstream iss(val.value());
        int valInt;
        iss >> valInt;

        if (state.find(key) != state.end()) state[key] = valInt;
        else state.insert({key, valInt});

        worker->produceUp(constructMessage(OK, std::to_string(valInt), worker->getId()));
        return;
    }

    if (state.find(key) != state.end()) worker->produceUp(constructMessage(OK, std::to_string(state[key]), worker->getId()));
    else worker->produceUp(constructMessage(ERR, "Can't find key " + key, worker->getId()));
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

    while (true){
        std::optional<std::string> message = worker->consume();
        if (!message.has_value()) continue;

        std::string command;
        size_t workerId;
        std::vector<std::string> args;

        if (parseMessage(message.value(), command, workerId, args, worker)) continue;

        if (command == "kill"){
            if (checkIfMessageIsForMe(message.value(), workerId, worker)) break;
            continue;
        }

        if (command == "ping"){
            if (!checkIfMessageIsForMe(message.value(), workerId, worker)) continue;
            handlePing(worker);
            continue;
        }

        if (command == "attach"){
            if (!checkIfMessageIsForMe(message.value(), workerId, worker)) continue;
            if (args.size() != 1) {
                worker->produceUp(constructMessage(ERR, "Message is malformed", worker->getId()));
                continue;
            }
            handleAttach(worker, args[0]);
            continue;
        }

        if (command == "exec"){
            if (!checkIfMessageIsForMe(message.value(), workerId, worker)) continue;
            if (args.empty()) {
                worker->produceUp(constructMessage(ERR, "Message is malformed", worker->getId()));
                continue;
            }
            std::optional<std::string> val = std::nullopt;
            if (args.size() == 2) val = args[1];

            handleExec(state, args[0], val, worker);
            continue;
        }

    }
}