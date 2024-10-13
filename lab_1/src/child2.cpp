#include <string>
#include <csignal>
#include <iostream>
#include "utils.h"

int main(int argv, char ** argc){
    FILE * file = fopen(argc[0], "w");
    dup2(fileno(file), 1);

    ReadData([](const std::string& str){
        std::string res = Modify(str);
        write(1, res.c_str(), res.size());
    }, std::cin);

    fclose(file);
    close(1);
}