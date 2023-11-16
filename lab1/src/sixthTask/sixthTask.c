#include "sixthTask.h"

void sixthTask(char** argv) {
    int filePointer = createFile(argv);

    int readOutput, writeOutput, closeOutput, position;
    char *text = "aboba";
    int textSize = strlen(text);

    writeOutput = write(filePointer, text, textSize);
    position = lseek(filePointer, 5L, SEEK_END);
    writeOutput = write(filePointer, text, textSize);
    closeOutput = close(filePointer);

    filePointer = open(argv[1], O_RDONLY);

    char textRead[2*textSize + NULL_OFFSET];
    position = lseek(filePointer, 0L, SEEK_SET);
    readOutput = read(filePointer, textRead, 2*textSize + NULL_OFFSET);
    printf("Output from file is %s", textRead);
    closeOutput = close(filePointer);

    remove(argv[1]);
}
