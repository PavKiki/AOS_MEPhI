#include "secondTask.h"

// char fileName2[20];
// int privileges;

const char* text = "I forgot what is C.\nI believe I would never\nUse it again\n";
char* textRead;

int closeOutput, readOutput;

void secondTask(char** argv) {
    int filePointer = createFile(argv);
    int bytesWritten = writeTextInFile(&filePointer);
    readAndOutputTextFromFile(argv, &filePointer, bytesWritten);
    openTextFileOnRW(argv, &filePointer);

    remove(argv[1]);
}

int createFile(char** argv) {
    // printf("Enter the name of the file: ");
    // scanf("%s", fileName2);

    // printf("Enter the level of access: ");
    // scanf("%d", &privileges);
    
    int filePointer = creat(argv[1], strtol(argv[2], NULL, 8));
    //3rd task
    printf("creat() output is %d\n", filePointer);

    return filePointer;
}

int writeTextInFile(int *filePointer) {
    size_t textSize = strlen(text);
    int bytesWritten = write(*filePointer, text, textSize);
    //3rd task
    printf("write() output is %d\n", bytesWritten);
    
    closeOutput = close(*filePointer);
    //3rd task
    printf("close() output is %d\n", closeOutput);

    return bytesWritten;
}

void readAndOutputTextFromFile(char** argv, int *filePointer, int bytesToRead) {
    // filePointer = open(fileName2, O_RDONLY);
    *filePointer = open(argv[1], O_RDONLY);
    //3rd task
    printf("open() output is %d\n", *filePointer);
    
    textRead = malloc(bytesToRead);
    readOutput = read(*filePointer, textRead, bytesToRead);
    //3rd task
    printf("read() output is %d\n", readOutput);
    
    printf("The text from the file is: \n%s", textRead);
    printf("\n");

    free(textRead);
    closeOutput = close(*filePointer);
    //3rd task
    printf("close() output is %d\n", closeOutput);
}

void openTextFileOnRW(char** argv, int *filePointer) {
    *filePointer = open(argv[1], O_RDWR);
    printf("open() output is %d\n", *filePointer);
    perror("The error is");

    closeOutput = close(*filePointer);
    //3rd task
    printf("close() output is %d\n", closeOutput);
}