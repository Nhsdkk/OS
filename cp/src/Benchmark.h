//
// Created by nikit on 12/23/2024.
//

#ifndef OS_CP_SRC_BENCHMARK_H_
#define OS_CP_SRC_BENCHMARK_H_

#include <type_traits>
#include <cstddef>
#include <utility>
#include <map>
#include <cstring>
#include <fstream>
#include "iostream"
#include "BaseMemoryResource.h"
#include "RandomNumberGenerator.h"
#include "TestClass.h"

enum class Type{
    INT,
    STRING,
    VECTOR_INT,
    CLASS
};

enum class Command{
    ALLOCATE,
    DEALLOCATE
};

std::string commandToString(const Command command) {
    switch (command) {
        case Command::ALLOCATE:
            return "ALLOCATE";
        case Command::DEALLOCATE:
            return "DEALLOCATE";
    }
    throw std::runtime_error("Can't convert command to string");
}

namespace Benchmark{
    template<class T>
    requires std::is_default_constructible_v<T>
    std::tuple<T*, double> alloc(Memory::BaseMemoryResource* memory_resource){
        std::pmr::polymorphic_allocator<T> allocator(memory_resource);
        auto [time, obj] = Utils::measure_time<T*>([&allocator](){
            return new (allocator.allocate(1)) T();
        });
        return {obj, time};
    }

    template<>
    std::tuple<char*, double> alloc<char>(Memory::BaseMemoryResource* memory_resource){
        std::pmr::polymorphic_allocator<char> allocator(memory_resource);
        auto [time, obj] = Utils::measure_time<char*>([&allocator](){
          return allocator.allocate(20);
        });
        std::memcpy(obj, "Hello, World!", 14);
        return {obj, time};
    }


    std::tuple<int*, double> allocIntArray(Memory::BaseMemoryResource* memory_resource){
        std::pmr::polymorphic_allocator<int> allocator(memory_resource);
        auto [time, obj] = Utils::measure_time<int*>([&allocator](){
          return allocator.allocate(20);
        });
        return {obj, time};
    }

    template<class T>
    requires std::is_default_constructible_v<T>
    double deallocate(T* obj, Memory::BaseMemoryResource* memory_resource){
        std::pmr::polymorphic_allocator<T> allocator(memory_resource);
        auto [time, _] = Utils::measure_time<int>([&allocator, &obj](){
            allocator.deallocate(obj, 1);
            return 1;
        });
        return time;
    }

    class Wrapper {
        size_t id;
        Type type;
        Memory::BaseMemoryResource* memory_resource;
        void* obj;

        public:
            Wrapper(const Type type, void* obj, const size_t id, Memory::BaseMemoryResource* memory_resource): type(type), obj(obj), id(id), memory_resource(memory_resource) {}
            Type getType() const { return type; }
            template<class T>
            T* getObj() const { return (T*)obj; }
            bool operator==(const Wrapper& other) const { return id == other.id; }
            bool isEmpty() const { return obj == nullptr; }
            double deallocate(){
                if (isEmpty()) return 0;
                switch (type) {
                    case Type::INT: {
                        auto time = Benchmark::deallocate<int>((int *) obj, memory_resource);
                        obj = nullptr;
                        return time;
                    }
                    case Type::STRING: {
                        auto time = Benchmark::deallocate<char>((char *) obj, memory_resource);
                        obj = nullptr;
                        return time;
                    }
                    case Type::VECTOR_INT: {
                        auto time = Benchmark::deallocate<int>((int *) obj, memory_resource);
                        obj = nullptr;
                        return time;
                    }
                    case Type::CLASS: {
                        auto time = Benchmark::deallocate<TestInternal::TestClass>((TestInternal::TestClass *) obj,
                                                                                   memory_resource);
                        obj = nullptr;
                        return time;
                    }
                }
                throw std::runtime_error("Can't deallocate object");
            }
            ~Wrapper() {
                if (!isEmpty()) deallocate();
            }
    };

    Type typeIntToTypeEnum(int type) { return static_cast<Type>(type); }

    Type generateRandomType(){
        auto rnd = Utils::RandomNumberGenerator();
        return typeIntToTypeEnum(rnd.generateInt(0, 3));
    }

    std::tuple<Wrapper*, double> allocateRandomObject(Memory::BaseMemoryResource* memory_resource, size_t id){
        auto type = generateRandomType();

        switch (type) {
            case Type::INT: {
                auto [ptr, time] = alloc<int>(memory_resource);
                *ptr = 155;
                auto obj = new Wrapper{type, (void *) ptr, id, memory_resource};
                return {obj, time};
            }
            case Type::STRING: {
                auto [ptr, time] = alloc<char>(memory_resource);
                auto obj = new Wrapper{type, (void *) ptr, id, memory_resource};
                return {obj, time};
            }
            case Type::VECTOR_INT:{
                auto [ptr, time] = allocIntArray(memory_resource);
                auto obj = new Wrapper{type, (void *) ptr, id, memory_resource};
                return {obj, time};
            }
            case Type::CLASS: {
                auto [ptr, time] = alloc<TestInternal::TestClass>(memory_resource);
                auto obj = new Wrapper{type, (void *) ptr, id, memory_resource};
                return {obj, time};
            }
        }

        throw std::runtime_error("Can't allocate object");
    }

    double deallocateRandomObject(std::vector<Wrapper*>& vec){
        if (vec.empty()) return 0;
        auto rnd = Utils::RandomNumberGenerator();
        auto rndIndex = rnd.generateInt(0, vec.size() - 1);
        auto time = vec[rndIndex]->deallocate();
        auto obj = vec[rndIndex];
        vec.erase(std::remove(vec.begin(), vec.end(), vec[rndIndex]), vec.end());
        std::free(obj);
        return time;
    }

    void formatOutput(std::ofstream& file, const double time, const Memory::BaseMemoryResource* memory_resource, const Command command){
        file << memory_resource->getAllocatedObjects() << " ";
        file << time << " ";
        file << memory_resource->getTotalAllocatedMemory() << " ";
        file << memory_resource->getTotalUsedMemory() << " ";
        file << commandToString(command) << std::endl;
    }

    void createHeader(std::ofstream& file){
        file << "objectsCount time allocatedMemory usedMemory commandType" << std::endl;
    }

    void benchmark_allocator(const std::string& outputFileName, Memory::BaseMemoryResource* memory_resource, std::vector<Command> commands, size_t baseObjectCount = 100){
        std::ofstream file;
        file.open(outputFileName, std::ios_base::app);

        createHeader(file);

        auto idx = 0;

        std::vector<Wrapper*> objects;
        for (auto i = 0; i < baseObjectCount; ++i){
            auto [obj, time] = allocateRandomObject(memory_resource, idx);
            idx++;
            objects.push_back(obj);
            formatOutput(file, time, memory_resource, Command::ALLOCATE);
        }

        for (auto command: commands){
            switch (command) {
                case Command::ALLOCATE: {
                    auto [obj, time] = allocateRandomObject(memory_resource, idx);
                    idx++;
                    objects.push_back(obj);
                    formatOutput(file, time, memory_resource, Command::ALLOCATE);
                    break;
                }
                case Command::DEALLOCATE: {
                    auto time = deallocateRandomObject(objects);
                    formatOutput(file, time, memory_resource, Command::DEALLOCATE);
                    break;
                }

            }
        }

        for (auto &object : objects) {
            auto time = object->deallocate();
            std::free(object);
            formatOutput(file, time, memory_resource, Command::DEALLOCATE);
        }
        file << std::endl;

    }
}

#endif //OS_CP_SRC_BENCHMARK_H_
