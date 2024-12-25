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
    size_t id;
    std::string command;
    std::vector<std::string> args;
    int receiver;

    public:
        Request() : command("UNKNOWN"), args(std::vector<std::string>()), receiver(-1), id(0) {}
        Request(std::string command, std::vector<std::string> args, int receiver, size_t id) : command(std::move(command)), args(std::move(args)), receiver(receiver), id(id) {}
        std::string getCommand() { return command; }
        std::vector<std::string> getArgs() { return args; }
        int getReceiver() const { return receiver; }
        size_t getId() const { return id; }
        static Request fromStringRequest(const std::string& data) {
            std::vector<std::string> parts = split(data, ' ');
            if (parts.size() < 3) throw std::invalid_argument("Invalid request format");
            return {parts[1], parts.size() == 3 ? std::vector<std::string>() : std::vector<std::string>(parts.begin() + 3, parts.end()), fromString<int>(parts[2]), fromString<size_t>(parts[0])};
        }
        static Request fromStringRequest(const std::string& data, const size_t id) {
            std::vector<std::string> parts = split(data, ' ');
            if (parts.size() < 2) throw std::invalid_argument("Invalid request format");
            return {parts[0], parts.size() == 2 ? std::vector<std::string>() : std::vector<std::string>(parts.begin() + 2, parts.end()), fromString<int>(parts[1]), id};
        }

        std::string toString() {
            std::string result = std::to_string(id) + " " + command + " " + std::to_string(receiver);
            for (const auto& arg : args) result += " " + arg;
            return result;
        }
};

} // Request

#endif //OS_LAB_5_SRC_REQUEST_H_
