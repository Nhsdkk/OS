#pragma once

#include <iostream>
#include "unistd.h"
#include "constants.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore>
#include <functional>
#include <cstring>

int OpenSharedMemory(const std::string& name, bool exists, mode_t mode, size_t size) {
    auto result = shm_open(name.c_str(), mode | O_CREAT, 0777);
    if (!result) perror("Can't create/read shared memory object");

    if (exists) return result;

    if (ftruncate(result, size) == -1) perror("Can't resize shared memory object");
    return result;
}

class SharedObjectHandler {
    sem_t* semChild;
    sem_t* semParent;
    char* buffer;
    size_t bufferSize;
    std::string semNameChild, semNameParent, objectName;

    public:
        SharedObjectHandler(const std::string& sNameChild, const std::string& objName, const mode_t fdMode = O_RDONLY, const int prot = PROT_READ, const bool exits = true, const size_t sz = BUFFER_SIZE){
            auto fd = OpenSharedMemory(objName, exits, fdMode, sz);
            buffer = static_cast<char*>(mmap(nullptr, sz, prot, MAP_SHARED, fd, 0));
            bufferSize = sz;
            semNameChild = sNameChild;
            semNameParent = sNameChild + "_parent";
            objectName = objName;
            semChild = sem_open(semNameChild.c_str(), O_CREAT, O_RDWR , 0);
            semParent = sem_open(semNameParent.c_str(), O_CREAT, O_RDWR , 0);
        }

        std::string Read() {
            std::string buff {buffer};
            return buff;
        }

        void Write(const std::string& data) const {
            if (data.size() >= bufferSize) perror("Buffer overflow");
            strcpy(buffer, data.c_str());
        }

        void DestroySem() const {
            sem_unlink(semNameParent.c_str());
            sem_unlink(semNameChild.c_str());
        }

        void LockParent() { sem_wait(semParent); }
        void LockChild() { sem_wait(semChild); }

        void UnlockParent() { sem_post(semParent); }
        void UnlockChild() { sem_post(semChild); }

        ~SharedObjectHandler() {
            munmap(buffer, bufferSize);
            shm_unlink(objectName.c_str());
        }
};

