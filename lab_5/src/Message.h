//
// Created by nikit on 12/24/2024.
//

#ifndef OS_LAB_5_SRC_MESSAGE_H_
#define OS_LAB_5_SRC_MESSAGE_H_

#include <cstdio>
#include <string>
#include <optional>
#include <utility>
#include <vector>
#include <sstream>

namespace Message{
    enum MessageType {
        INPUT,
        OUTPUT
    };

    class Message {
        MessageType type;
        std::vector<std::string> commandText;
        std::optional<std::vector<std::string>> resultText;
        size_t targetId;

        public:
            Message(
                MessageType type,
                std::vector<std::string> commandText,
                std::optional<std::vector<std::string>> resultText,
                size_t targetId
            ) : type(type), commandText(std::move(commandText)), targetId(targetId), resultText(std::move(resultText)) {}

            MessageType getType() const { return type; }
            const std::vector<std::string>& getCommandTextSplitted() const { return commandText; }
            const std::string& getCommandText() const {
                std::stringstream result;
                for (auto& str: commandText){
                    result << str << " ";
                }
                std::string r = result.str();

                if (!r.empty()) {
                    r.pop_back();
                }

                return r;
            }
            const std::string& getResultText() const {
                std::stringstream result;
                for (auto& str: resultText.value()){
                    result << str << " ";
                }
                std::string r (result.str());

                if (!r.empty()) {
                    r.pop_back();
                }

                return r;
            }
            const std::optional<std::vector<std::string>>& getResultTextSplitted() const { return resultText; }
            size_t getTargetId() const { return targetId; }
    };

} // Message

#endif //OS_LAB_5_SRC_MESSAGE_H_
