//
// Created by nikit on 12/24/2024.
//

#include <fstream>
#include "src/Utils.h"
#include "src/Worker.h"

int main(int argc, char** argv) {
    if (argc != 3) {
        return -1;
    }

    auto id = fromString<int>(argv[0]);
    auto parentPortInput = fromString<int>(argv[1]);
    auto parentPortOutput = fromString<int>(argv[2]);

    auto worker = Worker::Worker(id, parentPortInput, parentPortOutput);

    auto receiver = [](zmq::socket_t* parent){
        auto message = net::receive(parent);
        return Request::Request::fromStringRequest(message);
    };

    auto sender = [](zmq::socket_t* parent, const std::string& message){
        net::send_message(parent, message);
    };

    worker.run(receiver, sender);
}