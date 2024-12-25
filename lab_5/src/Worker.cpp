//
// Created by nikit on 12/24/2024.
//

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <variant>
#include <thread>
#include "Worker.h"
#include "Utils.h"
#include "constants.h"
#include "Request.h"

namespace Worker {
    Response::Response Worker::handleExec(const std::vector<std::string>& args) {
        if (args.size() == 2) {
            if (data.contains(args[0])) data[args[0]] = fromString<int>(args[1]);
            else data.insert({args[0], fromString<int>(args[1])});
            return {Response::ResponseType::SUCCESS, args[1], id};
        }
        if (args.size() != 1) return Response::Response::generateMalformedResponse(id);
        if (data.contains(args[0])) return {Response::ResponseType::SUCCESS, std::to_string(data[args[0]]), id};
        return {Response::ResponseType::FAILURE, args[0] + " key does not exist", id};
    }

    std::variant<Response::Response, Option> Worker::handleCommands(
        const std::string& command,
        const int receiver,
        const std::vector<std::string>& args
    ) {
        if (command == "kill") {
            if (receiver == -1 && id == -1) return Option::BREAK;
            if (receiver != -2) {
                if (!children.contains(receiver)) return Option::PASS_MESSAGE;
                return handleKill(receiver);
            }
        }

        if (receiver != id) {
            {
                std::ofstream file("worker_" + std::to_string(id) + ".log", std::ios::app);
                file << "PASSING MESSAGE TO CHILD " << std::endl;
            }
            return Option::PASS_MESSAGE;
        }

        if (command == "create") {
            return handleCreate(args);
        }  else if (command == "exec") {
            return handleExec(args);
        } else if (command == "ping"){
            return handlePing();
        } else if (command == "kill") {
            return handleKill(receiver);
        }

        std::cout << "Unknown command" << std::endl;
        return Response::Response::generateUnknownCommandResponse(id);
    }

    void Worker::run(const std::function<std::string(zmq::socket_t*)>& receiveData, const std::function<void(zmq::socket_t*, std::string)>& sendResult) {
        bool finished = false;
        std::thread workerThread([&sendResult, this, &finished](){
            std::string result;
            while(!finished) {
                try {
                    result = net::receive(&current);
                    std::cout << "Received result at " << id << ": " << result << std::endl;
                    sendResult(&parentOutput, result);
                } catch (...) {
                    continue;
                }
            }
        });

        std::string stringCommand;
        while (true) {
            try {
                stringCommand = receiveData(&parentInput);
                std::cout << "Received command " << id << ": " << stringCommand << std::endl;
                auto request = Request::Request::fromStringRequest(stringCommand);
                auto response = handleCommands(request.getCommand(), request.getReceiver(), request.getArgs());
                {
                    std::ofstream file("worker_" + std::to_string(id) + ".log", std::ios::app);
                    if (std::holds_alternative<Response::Response>(response)) {
                        file << "response: " << std::get<Response::Response>(response).constructResponseString() << std::endl;
                    } else if (std::get<Option>(response) == Option::BREAK) {
                       file << "response: " << "BREAK" << std::endl;
                    } else if (std::get<Option>(response) == Option::PASS_MESSAGE) {
                        file << "response: " << "PASS_MESSAGE" << std::endl;
                    }
                }
                if (std::holds_alternative<Response::Response>(response)) {
                    {
                        std::ofstream file("worker_" + std::to_string(id) + ".log", std::ios::app);
                        file << "Sending result of request: " << stringCommand << " Message: " << std::get<Response::Response>(response).constructResponseString() << std::endl;
                    }
                    sendResult(&parentOutput, std::get<Response::Response>(response).constructResponseString());
                } else if (std::get<Option>(response) == Option::BREAK) {
                    break;
                } else if (std::get<Option>(response) == Option::PASS_MESSAGE) {
                    for (auto &child : children) {
                        {
                            std::ofstream file("worker_" + std::to_string(id) + ".log", std::ios::app);
                            file << "Passing request: " << stringCommand << " To port: " << childrenPorts[child.first] << std::endl;
                        }
                        net::send_message(child.second, stringCommand);
                    }
                }
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }

        std::cout << "Terminating worker with id " << id << std::endl;
    }

    Response::Response Worker::handlePing() const {
        return {Response::SUCCESS, "Ping successful", id};
    }

    Response::Response Worker::handleCreate(const std::vector<std::string> &args) {
        {
            std::ofstream file("worker_" + std::to_string(id) + ".log", std::ios::app);
            file << "handling create with arg size: " << args.size() << std::endl;
        }
        if (args.size() != 1) return Response::Response::generateMalformedResponse(id);

        {
            std::ofstream file("worker_" + std::to_string(id) + ".log", std::ios::app);
            file << "handling create with arg size(Passed arg check): " << args.size() << " They are: " <<args[0] << std::endl;
        }

        auto childId = fromString<int>(args[0]);
        children.insert({childId, new zmq::socket_t(context, ZMQ_DEALER)});
        auto port = net::bind(children[childId], childId);
        childrenPorts.insert({childId, port});
        auto pid = startProcess(WORKER_PATH, childId, childrenPorts[childId], currentPort);
        if (pid == -1) return {Response::FAILURE, "Failed to create process for worker with id " + args[0], id};
        children[childId]->setsockopt(ZMQ_RCVTIMEO, 3000);

        {
            std::ofstream file("worker_" + std::to_string(id) + ".log", std::ios::app);
            file << "childrenSize: " << children.size() << std::endl;
            file << "childrenPortSize: " << childrenPorts.size() << std::endl;
        }

        return {Response::SUCCESS, "Worker with id " + args[0] + " created (pid - " + std::to_string(pid) + ")", id};
    }

    Response::Response Worker::handleKill(int receiver) {
        if (receiver != -2) {
            Response::Response msg;
            try {
                net::send_message(children[receiver], Request::Request("kill", {}, -2).toString());
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            } catch (...) {}
            net::unbind(children[receiver], childrenPorts[receiver]);
            children[receiver]->close();
            children.erase(receiver);
            childrenPorts.erase(receiver);
            return {Response::ResponseType::SUCCESS, std::to_string(receiver) + " killed", id};
        }

        for (const auto& [childId, child] : children) {
            net::send_message(&parentOutput, Response::Response(Response::ResponseType::SUCCESS, "Killed", id).constructResponseString());
            net::send_message(child, Request::Request("kill", {}, -2).toString());
            net::unbind(child, childrenPorts[childId]);
            child->close();
        }
        children.clear();
        childrenPorts.clear();
        return {Response::ResponseType::SUCCESS, std::to_string(receiver) + " killed", id};
    }
} // Worker