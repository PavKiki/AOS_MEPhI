#include "eighthTask.h"

void eighthTask(int argc, char** argv) {
    char *from, *to;
    
    if (argc == 3) {
        from = argv[1];
        to = argv[2];
    } else {
        // task8/from.txt
        // task8/to.txt
        from = malloc(20);
        to = malloc(20);

        printf("Enter the source filename: ");
        scanf("%s", from);
        printf("Enter the destination filename: ");
        scanf("%s", to);
        printf("\n");

        printf("Correctness check: %s -> %s", from, to);
    }

    int fileFrom = open(from, O_RDONLY);
    int fileTo = open(to, O_RDWR);
    lseek(fileTo, 0L, SEEK_END);

    int eofFlag;
    char buffer;
    while (eofFlag = read(fileFrom, &buffer, 1) != 0) {
        write(fileTo, &buffer, eofFlag);
    }

    close(fileFrom);
    close(fileTo);
}