#include "seventhTask.h"

void seventhTask(char** argv) {
    //code masks http://www.ccfit.nsu.ru/~deviv/courses/unix/unix/ng7c229.html
    //hex to octal converter https://www.rapidtables.com/convert/number/hex-to-octal.html

    struct stat *buffer;
    int statOutput;
    
    buffer = malloc(sizeof(struct stat));
    statOutput = stat(argv[1], buffer);
    outputStatistics(buffer, argv[1]);
    free(buffer);
}

void outputStatistics(struct stat *buffer, char* fileName) {
    printf("\n%30s%15s\n", "Description", "Value");
    printf("%30s%15s\n", "FileName", fileName);
    printf("%30s%15o\n", "Code mask of this file", buffer->st_mode);
    printf("%30s%15lu\n", "Nubmer of disk device", buffer->st_dev);
    printf("%30s%15ld\n", "Size of the file", buffer->st_size);
    printf("%30s%15ld\n", "Last modification time", buffer->st_atime);
    printf("%30s%15ld\n", "Last modification time", buffer->st_ctime);
    printf("%30s%15ld\n", "Last modification time", buffer->st_mtime);
}