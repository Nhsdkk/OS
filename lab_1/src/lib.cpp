#include <iostream>
#include "lib.h"
#include <cstdio>
#include <cstdlib>
#include "unistd.h"
#include "utils.h"
#include "constants.h"
#include <sys/wait.h>
#include <vector>

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

int ParentMain(){
    int pipe_to_child_1[2];
    OpenPipe(pipe_to_child_1);

    int pipe_to_child_2[2];
    OpenPipe(pipe_to_child_2);

    std::string fName1, fName2;

    std::cin >> fName1 >> fName2;

    std::vector<std::string> data = ReadData();

    std::string more, less;

    for (const std::string& str : data){
        if (str.length() > MAX_STRING_LENGTH) more.append(str + '\n');
        else less.append(str + '\n');
    }

    if (StartProcess(pipe_to_child_2, CHILD_2_PATH, fName2)) return 0;
    write(pipe_to_child_2[WRITE_END], more.c_str(), more.length());
    close(pipe_to_child_2[WRITE_END]);

    if (StartProcess(pipe_to_child_1, CHILD_1_PATH, fName1)) return 0;
    write(pipe_to_child_1[WRITE_END], less.c_str(), less.length());
    close(pipe_to_child_1[WRITE_END]);

    wait(nullptr);
    wait(nullptr);


    return 0;
}
