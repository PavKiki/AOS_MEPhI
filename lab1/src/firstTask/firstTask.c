#include "firstTask.h"

extern int errno;

const char* fileName1 = "firstTask.txt";

void printErrors() {
    open(fileName1, O_RDONLY, 0777);
    printf("Error code via errno is %d\n", errno);

    // deprecated and excluded from errno.h
    // open(fileName, O_RDONLY, 0777);
    // printf("Error code via syserrlist is %d", sys_errlist[sysnerr-1]);

    open(fileName1, O_RDONLY, 0777);
    perror("Error via perror is");
    printf("\n");
}