//
// Created by nikit on 12/24/2024.
//

#include <iostream>
#include "src/Utils.h"
#include "src/Worker.h"

int main(int argc, char** argv) {
    auto worker = Worker::Worker();

    auto receiver = [](zmq::socket_t* parent){
        std::string buffer;
        std::cout << "Enter message: ";
        std::getline(std::cin, buffer, '\n');
        return buffer;
    };

    auto sender = [](zmq::socket_t* parent, const std::string& message){
        std::cout<< "Got message: " << message << std::endl;
    };

    worker.run(receiver, sender);
}