#include <string>
#include <iostream>
#include <vector>

#include "utils.h"
#include "constants.h"

const std::string vowels = "aeiouy";

std::vector<std::string> ReadData(){
    std::vector<std::string> strings;
    std::string buff;


    while (std::getline(std::cin, buff)){
        strings.push_back(buff);
    }

    return strings;
}

std::string Concat(std::vector<std:: string>& strings, char sep){
    std::string result;
    for (auto & s : strings){
        result += s + sep;
    }
    return result;
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