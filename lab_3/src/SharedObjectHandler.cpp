#include "SharedObjectHandler.h"

int OpenSharedMemory(const std::string& name, bool exists, mode_t mode, size_t size) {
    auto result = shm_open(name.c_str(), mode | O_CREAT, 0777);
    if (result == -1) perror("Can't create/read shared memory object");

    if (exists) return result;

    if (ftruncate(result, size) == -1) perror("Can't resize shared memory object");
    return result;
}

SharedObjectHandler::SharedObjectHandler(
    const std::string &sNameChild,
    const std::string &objName,
    const mode_t fdMode,
    const int prot,
    const bool exists,
    const size_t sz
): bufferSize(sz){
    auto fd = OpenSharedMemory(objName, exists, fdMode, sz);
    buffer = static_cast<char*>(mmap(nullptr, sz, prot, MAP_SHARED, fd, 0));
    if (buffer == (char*)(-1)) perror("Error while creating buffer.");
    semNameChild = sNameChild;
    semNameParent = sNameChild + "_parent";
    objectName = objName;
    semChild = sem_open(semNameChild.c_str(), O_CREAT, O_RDWR , 0);
    semParent = sem_open(semNameParent.c_str(), O_CREAT, O_RDWR , 0);
    if (semChild == SEM_FAILED || semParent == SEM_FAILED) perror("Some semaphores can't be constructed");
}

std::string SharedObjectHandler::Read() {
    std::string buff {buffer};
    return buff;
}

void SharedObjectHandler::Write(const std::string &data) {
    if (data.size() >= bufferSize) perror("Buffer overflow");
    strcpy(buffer, data.c_str());
}

void SharedObjectHandler::LockParent() { sem_wait(semParent); }
void SharedObjectHandler::LockChild() { sem_wait(semChild); }
void SharedObjectHandler::UnlockParent() { sem_post(semParent); }
void SharedObjectHandler::UnlockChild() { sem_post(semChild); }

SharedObjectHandler::~SharedObjectHandler() {
    munmap(buffer, bufferSize);
    shm_unlink(objectName.c_str());
    sem_unlink(semNameParent.c_str());
    sem_unlink(semNameChild.c_str());
}
