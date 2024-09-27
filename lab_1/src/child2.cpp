#include <string>
#include <iostream>
#include <fstream>
#include "utils.h"
#include "constants.h"

int main(int argv, char ** argc){
    auto strs = ReadData();

    auto i = 0;
    for (const auto& str : strs) {
        strs[i++] = Modify(str);
    }

    std::ofstream outfile (argc[READ_END]);
    outfile << Concat(strs) << std::endl;
    outfile.close();
}