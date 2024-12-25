//
// Created by nikit on 12/24/2024.
//

#ifndef OS_LAB_5_SRC_WORKER_H_
#define OS_LAB_5_SRC_WORKER_H_

#include "Response.h"
#include "../../cmake-build-debug/_deps/cppzmq-src/zmq.hpp"
#include "../../cmake-build-debug/_deps/zeromq-src/include/zmq.h"
#include "net.h"
#include <map>
#include <variant>
#include <fstream>

namespace Worker {

    enum Option {
        BREAK,
        PASS_MESSAGE
    };

    class Worker {
        int id = -1;
        zmq::context_t context;
        std::unordered_map<int, zmq::socket_t *> children;
        std::unordered_map<int, int> childrenPorts;
        zmq::socket_t parentInput;
        zmq::socket_t parentOutput;
        int parentPortInput;
        int parentPortOutput;

        zmq::socket_t current;
        int currentPort;

        std::map<std::string, int> data;

        private:
            std::variant<Response::Response, Option> handleCommands(
                const std::string& command,
                int receiver,
                const std::vector<std::string>& args
            );
            Response::Response handleExec(
                const std::vector<std::string>& args
            );
            Response::Response handleCreate(
                const std::vector<std::string>& args
            );
            Response::Response handlePing() const;
            Response::Response handleKill(int receiver);
        public:
            Worker() : id(-1), context(), children(), childrenPorts(), parentPortInput(-1), parentPortOutput(-1), parentInput(), parentOutput(), data() {
                current = zmq::socket_t(context, ZMQ_DEALER);
                currentPort = net::bind(&current, 0);
            }
            Worker(int id, int parentPortInput, int parentPortOutput) :
            id(id),
            context(),
            parentInput(context, ZMQ_DEALER),
            parentOutput(context, ZMQ_DEALER),
            parentPortInput(parentPortInput),
            parentPortOutput(parentPortOutput),
            children(),
            childrenPorts(),
            data()
            {
                if (id != -1){
                    net::connect(&parentOutput, parentPortOutput);
                    net::connect(&parentInput, parentPortInput);
                    {
                        std::ofstream file("worker_" + std::to_string(id) + ".log");
                        file << "Worker connected with id: " << id << " to parent portInput: " << parentPortInput << " to parent portOutput: "
                             << parentPortOutput << std::endl;
                    }
                }

                current = zmq::socket_t(context, ZMQ_DEALER);
                currentPort = net::bind(&current, id);
            }
            void run(const std::function<std::string(zmq::socket_t*)>& receiveData, const std::function<void(zmq::socket_t*, std::string)>& sendResult);
            ~Worker() = default;
    };

} // Worker

#endif //OS_LAB_5_SRC_WORKER_H_
