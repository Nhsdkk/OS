#include <string>
#include <iostream>
#include <fstream>
#include <csignal>
#include "utils.h"
#include "constants.h"

int main(int argv, char ** argc) {
    auto strs = ReadData();

    auto i = 0;
    for (const auto& str : strs) {
        strs[i++] = Modify(str);
    }

    FILE * file = fopen(argc[0], "w");
    dup2(fileno(file),1) ;

    auto str = Concat(strs);
    write(1, str.c_str(), str.size());

    fclose(file);
    close(1);
}