//
// Created by nikit on 12/4/2024.
//

#include <functional>
#include <istream>
#include <iostream>
#include <dlfcn.h>

const inline std::string pathToF1 = "./libf1.so";
const inline std::string pathToF2 = "./libf2.so";

typedef float (*PiFuncPointer)(int a);
typedef std::vector<int> (*SortFuncPointer)(std::vector<int> a);

void handlePi(const std::function<float (int)>& CalcPi, std::istream& is, std::ostream& os){
    int n;

    os << "Enter n: ";
    is >> n;

    auto result = CalcPi(n);
    os << "Result: " << result << std::endl;
}

void handleSort(const std::function<std::vector<int> (std::vector<int>)>& Sorter, std::istream& is, std::ostream& os){
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

void handlePiDynamic(const std::string& path, const std::string& funcName, std::istream& is, std::ostream& os) {
    void* libHandle = dlopen(path.c_str(), RTLD_LAZY);
    if (!libHandle) {
        os << "Error while loading library: " << dlerror() << std::endl;
        return;
    }

    auto pi = reinterpret_cast<PiFuncPointer>(dlsym(libHandle, funcName.c_str()));

    if (!pi) {
        os << "Error while loading library: Can't use function " << funcName << std::endl;
        return;
    }

    handlePi(pi, is, os);
    dlclose(libHandle);
}

void handleSortDynamic(const std::string& path, const std::string& funcName, std::istream& is, std::ostream& os) {
    void* libHandle = dlopen(path.c_str(), RTLD_LAZY);
    if (!libHandle) {
        os << "Error while loading library: " << dlerror() << std::endl;
        return;
    }

    auto pi = reinterpret_cast<SortFuncPointer>(dlsym(libHandle, funcName.c_str()));

    if (!pi) {
        os << "Error while loading library: Can't use function " << funcName << std::endl;
        return;
    }

    handleSort(pi, is, os);
    dlclose(libHandle);
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
                handlePiDynamic(pathToF2, "CalcPiVallis", is, os);
                break;
            case 2:
                handlePiDynamic(pathToF1, "CalcPiLeibniz", is, os);
                break;
            case 3:
                handleSortDynamic(pathToF2, "SortHoar", is, os);
                break;
            case 4:
                handleSortDynamic(pathToF1, "SortBubble", is, os);
                break;
            default:
                break;
        }

        if (command == 5) return 0;
    }
}