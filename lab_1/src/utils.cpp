#include <string>
#include <iostream>
#include <vector>
#include <csignal>

#include "utils.h"
#include "constants.h"

const std::string vowels = "aeiouy";

void ReadData(std::function<void(const std::string&)> handler){
    std::vector<std::string> strings;
    std::string buff;


    while (std::getline(std::cin, buff)){
        if (!buff.empty()) handler(buff + '\n');
    }
}

std::string Modify(const std::string& str){
    std::string res;

    for (auto& c : str){
        if (vowels.find(c) == ERROR){
            res += c;
        }
    }

    return res;
}

void OpenPipe(int *p) {
    if (pipe(p) == ERROR){
        perror("Can't open pipe");
        exit(ERROR);
    }
}