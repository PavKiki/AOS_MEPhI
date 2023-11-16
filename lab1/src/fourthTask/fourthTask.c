#include "fourthTask.h"

void fourthTask(char** argv) {

    int filePointer = createFile(argv);
    int bytesWritten = writeTextInFile(&filePointer);

    filePointer = open(argv[1], O_RDONLY);

    off_t position;
    int readOutput;
    char* textRead;

    //SEEK_SET
    position = lseek(filePointer, 10L, SEEK_SET);
    printf("lseek() output is %d\n", position);
    textRead = malloc(bytesWritten - position - 10);
    readOutput = read(filePointer, textRead, bytesWritten - position - 10);
    printf("read() output is %d\n", readOutput);
    printf("SEEK_SET output is: \n%s\n", textRead);

    free(textRead);

    //SEEK_CUR
    position = lseek(filePointer, 0L, SEEK_CUR);
    printf("lseek() output is %d\n", position);
    textRead = malloc(10);
    readOutput = read(filePointer, textRead, 10);
    printf("read() output is %d\n", readOutput);
    printf("SEEK_CUR output is: \n%s\n", textRead);

    free(textRead);

    //SEEK_END
    position = lseek(filePointer, -20L, SEEK_END);
    printf("lseek() output is %d\n", position);
    textRead = malloc(20);
    readOutput = read(filePointer, textRead, 20);
    printf("read() output is %d\n", readOutput);
    printf("SEEK_END output is: \n%s\n", textRead);

    free(textRead);

    remove(argv[1]);
}