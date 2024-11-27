#pragma once

#include <iostream>
#include "unistd.h"
#include "constants.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore>
#include <functional>
#include <cstring>

class SharedObjectHandler {
    sem_t* semChild;
    sem_t* semParent;
    char* buffer;
    const size_t bufferSize;
    std::string semNameChild, semNameParent, objectName;

    public:
        SharedObjectHandler(
            const std::string& sNameChild,
            const std::string& objName,
            const mode_t fdMode = O_RDONLY,
            const int prot = PROT_READ,
            bool exits = true,
            size_t sz = BUFFER_SIZE
        );

        std::string Read();

        void Write(const std::string& data);

        void LockParent();
        void LockChild();

        void UnlockParent();
        void UnlockChild();

        ~SharedObjectHandler();
};


