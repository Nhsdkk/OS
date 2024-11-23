#include <string>
#include <csignal>
#include "utils.h"
#include "constants.h"
#include "SharedObjectHandler.h"

int main(int argv, char ** argc) {
    auto handlerChild = SharedObjectHandler(CHILD_1_OBJECT_NAME, CHILD_1_OBJECT_NAME);

    FILE * file = fopen(argc[0], "w");
    if (!file){
        perror("Can't open file");
        return -1;
    }

    if (dup2(fileno(file), 1) == -1){
        perror("Can't redirect output");
        return -1;
    }

    std::string data;

    while (true){
        handlerChild.LockChild();

        data = handlerChild.Read();
        if (data == "\n") {
            handlerChild.UnlockChild();
            handlerChild.UnlockParent();
            break;
        }

        std::string res = Modify(data);
        write(1, res.c_str(), res.size());

        handlerChild.UnlockParent();
    }

    fclose(file);
}