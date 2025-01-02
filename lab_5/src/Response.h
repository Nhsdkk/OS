#include <utility>
#include <string>
#include <vector>
#include "Utils.h"
#include "Request.h"

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
    Request::Request request;

    public:
        Response() : type(FAILURE), message("Unknown error"), handlerId(-1), request() {};
        Response(ResponseType type, std::string message, int handlerId) : type(type), message(std::move(message)), handlerId(handlerId), request() {}
        std::string constructResponseString() {
            std::string stringType = type == SUCCESS ? "SUCCESS" : "FAILURE";
            return request.toString() + "\n" + stringType + " " + std::to_string(handlerId)  + " " + message;

        }
        static Response fromStringResponse(const std::string& data) {
            std::vector<std::string> rows = split(data, '\n');
            Request::Request req = Request::Request::fromStringRequest(rows[0]);
            std::vector<std::string> parts = split(rows[1], ' ');
            if (parts.size() < 3) throw std::invalid_argument("Invalid response format");
            auto response = Response(
                parts[0] == "SUCCESS" ? SUCCESS : FAILURE,
                concat(std::vector<std::string>(parts.begin() + 2, parts.end())),
                fromString<int>(parts[1])
            );
            response.setRequest(req);
            return response;
        }
        static Response generateMalformedResponse(int handlerId) {
            return {FAILURE, "Message malformed", handlerId};
        }
        static Response generateUnknownCommandResponse(int handlerId) {
            return {FAILURE, "Unknown command", handlerId};
        }

        Request::Request getRequest() const { return request; }
        ResponseType getType() const { return type; }
        std::string getMessage() const { return message; }
        int getHandlerId() const { return handlerId; }

        void setRequest(const Request::Request& req) { Response::request = req; }
};

} // Response

#endif //OS_LAB_5_SRC_RESPONSE_H_
