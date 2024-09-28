#include <iostream>
#include "lib.h"
#include <cstdio>
#include <cstdlib>
#include "unistd.h"
#include "utils.h"
#include "constants.h"
#include <sys/wait.h>
#include <fstream>

std::string GetFileName(std::istream& stream){
    std::string buff;
    std::getline(stream, buff);
    return buff;
}

bool StartProcess(int * pipe, const std::string& childPath, std::string& filePath) {
    pid_t pid = fork();

    if (pid == ERROR){
        perror("Can't fork process");
        exit(ERROR);
    }

    if (pid == CHILD_PROCESS){
        close(pipe[WRITE_END]);
        dup2(pipe[READ_END], READ_END);

        if (execl(childPath.c_str(), const_cast<char *>(filePath.c_str()), NULL) == ERROR){
            std::cout << "Something went wrong when creating process " << childPath << std::endl;
        }
    }

    return pid == CHILD_PROCESS;
}

void ParentMain(std::istream& input){
    int pipe_to_child_1[2];
    OpenPipe(pipe_to_child_1);

    int pipe_to_child_2[2];
    OpenPipe(pipe_to_child_2);

    std::string fName1, fName2;

    std::cout << "Enter filename for 1 process: " << std::endl;
    fName1 = GetFileName(input);

    std::cout << "Enter filename for 2 process: " << std::endl;
    fName2 = GetFileName(input);

    std::cout << "FILE NAMES: "<< fName1 << " " << fName2 << std::endl;

    if (StartProcess(pipe_to_child_2, CHILD_2_PATH, fName2)) return;
    if (StartProcess(pipe_to_child_1, CHILD_1_PATH, fName1)) return;

    std::cout << "Enter strings to process: " << std::endl;

    ReadData([pipe_to_child_1, pipe_to_child_2](const std::string& str) {
        if (str.length() > MAX_STRING_LENGTH) write(pipe_to_child_2[WRITE_END], str.c_str(), str.size());
        else write(pipe_to_child_1[WRITE_END], str.c_str(), str.size());
    }, input);

    close(pipe_to_child_2[READ_END]);
    close(pipe_to_child_1[READ_END]);

    close(pipe_to_child_2[WRITE_END]);
    close(pipe_to_child_1[WRITE_END]);

    wait(nullptr);
    wait(nullptr);
}
