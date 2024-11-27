#include <string>
#include <vector>
#include <csignal>
#include <fstream>
#include <iostream>

#include "utils.h"
#include "constants.h"

const std::string vowels = "aeiouy";

std::string Modify(const std::string& str){
    std::string res;

    for (auto& c : str){
        if (vowels.find(c) == std::string::npos){
            res += c;
        }
    }

    return res;
}