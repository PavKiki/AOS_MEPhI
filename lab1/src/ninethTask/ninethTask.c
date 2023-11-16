#include "ninethTask.h"

void ninethTask(char** argv) {
    char *from, *to;
    from = argv[1];
    to = argv[2];

    int fileFrom = open(from, O_RDONLY);
    int fileTo = open(to, O_RDWR);
    lseek(fileTo, 0L, SEEK_END);

    dup2(fileFrom, STDIN_FILENO);
    dup2(fileTo, STDOUT_FILENO);

    cp();

    close(fileFrom);
    close(fileTo);
}

void cp() {
    int eofFlag;
    char buffer;
    while (eofFlag = read(STDIN_FILENO, &buffer, 1) != 0) {
        write(STDOUT_FILENO, &buffer, eofFlag);
    }
}