#include "tenthTask.h"

void tenthTask(char** argv) {
    char *from;
    from = argv[1];

    int fileFrom = open(from, O_RDONLY);
    lseek(fileFrom, 1L, SEEK_END);

    int eofFlag;
    char buffer;
    while (eofFlag = lseek(fileFrom, -2L, SEEK_CUR) != -1) {
        read(fileFrom, &buffer, 1);
        printf("%c", buffer);
    }

    close(fileFrom);
}