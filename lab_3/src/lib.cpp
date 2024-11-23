#include <iostream>
#include "lib.h"
#include <cstdio>
#include <cstdlib>
#include "unistd.h"
#include "utils.h"
#include "constants.h"
#include "SharedObjectHandler.h"
#include <sys/wait.h>
#include <sys/mman.h>

std::string GetFileName(std::istream& stream){
    std::string buff;
    std::getline(stream, buff);
    return buff;
}

bool StartProcess(const std::string& childPath, std::string& filePath) {
    pid_t pid = fork();

    if (pid == ERROR){
        perror("Can't fork process");
        exit(ERROR);
    }

    if (pid == CHILD_PROCESS){
        if (execl(childPath.c_str(), const_cast<char *>(filePath.c_str()), NULL) == ERROR){
            std::cout << "Something went wrong when creating process " << childPath << std::endl;
        }
    }

    return pid == CHILD_PROCESS;
}

void ParentMain(std::istream& input){
    auto handlerChild1 = SharedObjectHandler(CHILD_1_OBJECT_NAME, CHILD_1_OBJECT_NAME, O_RDWR, PROT_WRITE, false);
    auto handlerChild2 = SharedObjectHandler(CHILD_2_OBJECT_NAME, CHILD_2_OBJECT_NAME, O_RDWR, PROT_WRITE, false);


    std::string fName1, fName2;

    std::cout << "Enter filename for 1 process: " << std::endl;
    fName1 = GetFileName(input);

    std::cout << "Enter filename for 2 process: " << std::endl;
    fName2 = GetFileName(input);

    if (StartProcess(CHILD_2_PATH, fName2)) return;
    if (StartProcess(CHILD_1_PATH, fName1)) return;

    std::cout << "Enter strings to process: " << std::endl;

    std::string buff;

    while (std::getline(input, buff)){
        if (buff.empty()) {
            handlerChild2.Write("\n");
            handlerChild2.UnlockChild();
            handlerChild2.LockParent();
            handlerChild1.Write("\n");
            handlerChild1.UnlockChild();
            handlerChild1.LockParent();
            break;
        }

        if (buff.length() > MAX_STRING_LENGTH) {
            handlerChild2.Write(buff + '\n');
            handlerChild2.UnlockChild();
            handlerChild2.LockParent();
        }
        else {
            handlerChild1.Write(buff + '\n');
            handlerChild1.UnlockChild();
            handlerChild1.LockParent();
        }
    }

    wait(nullptr);
    wait(nullptr);
}
