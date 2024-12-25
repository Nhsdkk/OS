//
// Created by nikit on 12/24/2024.
//

#include <iostream>
#include <set>
#include <thread>
#include "src/Utils.h"
#include "src/Worker.h"
#include "src/Tree.h"

int main(int argc, char** argv) {
    auto worker = Worker::Worker();

    size_t id = 0;

    std::set<size_t> unfinishedRequests;
    Tree::Tree<int> tree(-1);

    auto receiver = [&unfinishedRequests, &id, &tree](zmq::socket_t* parent){
        std::string buffer;
        std::cout << "Enter message: ";
        std::getline(std::cin, buffer, '\n');
        auto request = Request::Request::fromStringRequest(buffer, id);

        if (request.getCommand() == "create" && tree.exists(fromString<int>(request.getArgs()[0]))) {
            std::cout << "Node with id " << fromString<int>(request.getArgs()[0]) << " already exists" << std::endl;
            throw std::invalid_argument("Node with id " + std::to_string(fromString<int>(request.getArgs()[0])) + " already exists");
        } else if (request.getCommand() == "create" && !tree.exists(request.getReceiver())) {
            std::cout << "Node with id " << request.getReceiver() << " does not exist (Receiver)" << std::endl;
            throw std::invalid_argument("Node with id " + std::to_string(request.getReceiver()) + " does not exist (Receiver)");
        }else if (request.getCommand() == "kill" && !tree.exists(request.getReceiver())) {
            std::cout << "Node with id " << request.getReceiver() << " does not exist" << std::endl;
            throw std::invalid_argument("Node with id " + std::to_string(request.getReceiver()) + " does not exist");
        }

        unfinishedRequests.insert(request.getId());

        std::thread thread([&unfinishedRequests, &request, &tree](){
            auto start = std::chrono::steady_clock::now();
            while (std::chrono::steady_clock::now() - start < TIMEOUT) {
                if (unfinishedRequests.contains(request.getId())) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                } else {
                    return;
                }
            }

            tree.remove(request.getReceiver());
            unfinishedRequests.erase(request.getId());
            std::cout << "Request \"" << request.toString() << "\" timed out" << std::endl;
        });

        id++;

        return request;
    };

    auto sender = [&tree, &unfinishedRequests](zmq::socket_t* parent, const std::string& message){
        Response::Response response = Response::Response::fromStringResponse(message);
        auto req = response.getRequest();
        if (!unfinishedRequests.contains(req.getId())) { return; }
        std::cout<< "Got message: " << split(response.constructResponseString(), '\n')[1] << std::endl;
        if (req.getCommand() == "create"){
            tree.attach(req.getReceiver(), fromString<int>(req.getArgs()[0]));
        } else if (req.getCommand() == "kill") {
            tree.remove(req.getReceiver());
        }

        unfinishedRequests.erase(req.getId());
    };

    worker.run(receiver, sender);
}