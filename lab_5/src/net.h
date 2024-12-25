//
// Created by nikit on 12/24/2024.
//

#ifndef OS_LAB_5_SRC_NET_H_
#define OS_LAB_5_SRC_NET_H_

#include <iostream>
#include "../../cmake-build-debug/_deps/cppzmq-src/zmq.hpp"
#include "constants.h"

namespace net {
    inline int bind(zmq::socket_t *socket, int id) {
        int port = basePort + id;
        while (true) {
            std::string adress = baseAddress + std::to_string(port);
            try {
                socket->bind(adress);
                break;
            } catch (std::exception &e) {
                port++;
            }
        }
        return port;
    }

    inline void connect(zmq::socket_t *socket, int port) {
        std::string adress = baseAddress + std::to_string(port);
        socket->connect(adress);
    }

inline void unbind(zmq::socket_t *socket, int port) {
        std::string adress = baseAddress + std::to_string(port);
        socket->unbind(adress);
    }

inline void disconnect(zmq::socket_t *socket, int port) {
        std::string adress = baseAddress + std::to_string(port);
        socket->disconnect(adress);
    }

inline void send_message(zmq::socket_t *socket, const std::string msg) {
        zmq::message_t message(msg.size());
        memcpy(message.data(), msg.c_str(), msg.size());
        try {
            socket->send(message);
        } catch (...) {}
    }

inline std::string receive(zmq::socket_t *socket) {
        zmq::message_t message;
        bool success = true;
        try {
            socket->recv(&message, 0);
        } catch (...) {
            success = false;
        }
        if (!success || message.empty()) {
            throw std::runtime_error("Error while receiving message");
        }
        std::string str(static_cast<char *>(message.data()), message.size());
        return str;
    }
}

#endif //OS_LAB_5_SRC_NET_H_
