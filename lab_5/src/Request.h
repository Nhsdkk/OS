//
// Created by nikit on 12/24/2024.
//

#ifndef OS_LAB_5_SRC_REQUEST_H_
#define OS_LAB_5_SRC_REQUEST_H_

#include <string>
#include <utility>
#include <vector>
#include "Utils.h"
namespace Request {

class Request {
    std::string command;
    std::vector<std::string> args;
    int receiver;

    public:
        Request(std::string command, std::vector<std::string> args, int receiver) : command(std::move(command)), args(std::move(args)), receiver(receiver) {}
        std::string getCommand() { return command; }
        std::vector<std::string> getArgs() { return args; }
        int getReceiver() const { return receiver; }
        static Request fromStringRequest(const std::string& data) {
            std::vector<std::string> parts = split(data, ' ');
            if (parts.size() < 2) throw std::invalid_argument("Invalid request format");
            return {parts[0], parts.size() == 2 ? std::vector<std::string>() : std::vector<std::string>(parts.begin() + 2, parts.end()), fromString<int>(parts[1])};
        }
        std::string toString() {
            std::string result = command + " " + std::to_string(receiver);
            for (const auto& arg : args) result += " " + arg;
            return result;
        }
};

} // Request

#endif //OS_LAB_5_SRC_REQUEST_H_
