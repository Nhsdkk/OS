#include <string>
#include <vector>
#include <csignal>
#include <fstream>
#include <iostream>

#include "utils.h"
#include "constants.h"

const std::string vowels = "aeiouy";

void ReadData(const std::function<void(const std::string&)>& handler, std::basic_istream<char>& stream){
    std::string buff;

    while (std::getline(stream, buff)){
        if (buff.empty()){
            return;
        }
        handler(buff + '\n');
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