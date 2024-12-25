#include <utility>
#include <string>
#include <vector>
#include "Utils.h"

//
// Created by nikit on 12/24/2024.
//

#ifndef OS_LAB_5_SRC_RESPONSE_H_
#define OS_LAB_5_SRC_RESPONSE_H_

inline std::string concat(const std::vector<std::string>& data) {
    std::string result;
    for (const auto &item : data) result += item + " ";
    if (!result.empty()) {
        result.pop_back();
    }
    return result;
}

namespace Response {

enum ResponseType {
    SUCCESS,
    FAILURE
};

class Response {
    ResponseType type;
    std::string message;
    int handlerId{};

    public:
        Response() : type(FAILURE), message("Unknown error"), handlerId(-1) {};
        Response(ResponseType type, std::string message, int handlerId) : type(type), message(std::move(message)), handlerId(handlerId) {}
        std::string constructResponseString() {
            std::string stringType = type == SUCCESS ? "SUCCESS" : "FAILURE";
            return stringType + " " + std::to_string(handlerId)  + " " + message;
        }
        static Response fromStringResponse(const std::string& data) {
            std::vector<std::string> parts = split(data, ' ');
            if (parts.size() < 3) throw std::invalid_argument("Invalid response format");
            return {parts[0] == "SUCCESS" ? SUCCESS : FAILURE, concat(std::vector<std::string>(parts.begin() + 2, parts.end())), fromString<int>(parts[1])};
        }
        static Response generateMalformedResponse(int handlerId) {
            return {FAILURE, "Message malformed", handlerId};
        }
        static Response generateUnknownCommandResponse(int handlerId) {
            return {FAILURE, "Unknown command", handlerId};
        }
};

} // Response

#endif //OS_LAB_5_SRC_RESPONSE_H_
