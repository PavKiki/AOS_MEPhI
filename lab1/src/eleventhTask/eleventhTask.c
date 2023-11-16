#include "eleventhTask.h"

void eleventhTask(int argc, char** argv) {
    if (argc == 1) return;

    int *fileLength = calloc(argc - 1, sizeof(long));
    
    for (int i = 1; i < argc; i++) {
        int filePointer = open(argv[i], O_RDONLY);
        fileLength[i - 1] = lseek(filePointer, 0L, SEEK_END); 
    }

    int max = fileLength[0];
    for (int i = 1; i < argc - 1; i++) {
        if (max < fileLength[i]) max = fileLength[i];
    }

    printf("Max length is %d\n", max);
}