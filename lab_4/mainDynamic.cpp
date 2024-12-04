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

void handlePi(const std::function<float (int)>& CalcPi){
    int n;

    std::cout << "Enter n: ";
    std::cin >> n;

    auto result = CalcPi(n);
    std::cout << "Result: " << result << std::endl;
}

void handleSort(const std::function<std::vector<int> (std::vector<int>)>& Sorter){
    std::vector<int> vec;
    size_t size;

    std::cout << "Enter size: ";
    std::cin >> size;

    std::cout << "Enter values separated by space: ";
    int val;
    for (auto i = 0; i < size; ++i){
        std::cin >> val;
        vec.push_back(val);
    }

    std::cout << "Sorting..." << std::endl;
    auto result = Sorter(vec);
    std::cout << "Result: ";
    for (auto& item: result) std::cout << item << " ";
    std::cout << std::endl;
}

void handlePiDynamic(void* libHandle, const std::string& funcName) {
    auto pi = reinterpret_cast<PiFuncPointer>(dlsym(libHandle, funcName.c_str()));

    if (!pi) {
        std::cout << "Error while loading library: Can't use function " << funcName << std::endl;
        return;
    }

    handlePi(pi);
    dlclose(libHandle);
}

void handleSortDynamic(void* libHandle, const std::string& funcName) {
    auto pi = reinterpret_cast<SortFuncPointer>(dlsym(libHandle, funcName.c_str()));

    if (!pi) {
        std::cout << "Error while loading library: Can't use function " << funcName << std::endl;
        return;
    }

    handleSort(pi);
    dlclose(libHandle);
}

void* loadLib(const std::string& path){
    void* libHandle = dlopen(path.c_str(), RTLD_LAZY);
    if (!libHandle) {
        std::cout << "Error while loading library: " << dlerror() << std::endl;
        return nullptr;
    }
    return libHandle;
}

int main(int argv, char** argc){
    int command = 0;

    void* libHandle = dlopen(pathToF1.c_str(), RTLD_LAZY);
    bool isF1 = true;
    
    while (command != 5) {
        std::cout << "Enter command: ";
        std::cin >> command;

        switch (command) {
            case 0:
                if (isF1) {
                    libHandle = loadLib(pathToF2);
                    isF1 = false;
                }
                else {
                    libHandle = loadLib(pathToF1);
                    isF1 = true;
                }
                break;
            case 1:
                handlePiDynamic(libHandle, isF1 ? "CalcPiLeibniz" : "CalcPiVallis");
                break;
            case 2:
                handleSortDynamic(libHandle, isF1 ? "SortBubble" : "SortHoar");
                break;
            default:
                break;
        }

        if (command == 3) return 0;
    }
}