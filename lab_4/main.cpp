#include <istream>
#include <vector>
#include <iostream>
#include <functional>
#include "src/f1.h"
#include "src/f2.h"

using PiFunc = std::function<float (int)>;
using Sorter = std::function<std::vector<int> (std::vector<int>)>;

void handlePi(const PiFunc& CalcPi, std::istream& is, std::ostream& os){
    int n;

    os << "Enter n: ";
    is >> n;

    auto result = CalcPi(n);
    os << "Result: " << result << std::endl;
}

void handleSort(const Sorter& Sorter, std::istream& is, std::ostream& os){
    std::vector<int> vec;
    size_t size;

    os << "Enter size: ";
    is >> size;

    os << "Enter values separated by space: ";
    int val;
    for (auto i = 0; i < size; ++i){
        is >> val;
        vec.push_back(val);
    }

    os << "Sorting..." << std::endl;
    auto result = Sorter(vec);
    os << "Result: ";
    for (auto& item: result) os << item << " ";
    os << std::endl;
}

int main(int argv, char** argc){
    int command = 0;
    std::istream& is = std::cin;
    std::ostream& os = std::cout;
    while (command != 5) {
        os << "Enter command: ";
        is >> command;

        switch (command) {
            case 1:
                handlePi(CalcPiVallis, is, os);
                break;
            case 2:
                handlePi(CalcPiLeibniz, is, os);
                break;
            case 3:
                handleSort(SortHoar, is, os);
                break;
            case 4:
                handleSort(SortBubble, is, os);
                break;
            default:
                break;
        }

        if (command == 5) return 0;
    }
}
