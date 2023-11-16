#include "8task.h"

int main(int argc, char** argv) {
    int filePointer = open(argv[1], O_RDONLY);

    int pid_child = fork();

    switch (pid_child)
    {
    case 0:
        //child
        {
            char* fileName = createFileNameWithPostfix(argv, "_child");
            int fileDescriptor = createReadOnlyFile(fileName);
            readFileIntoFile(filePointer, fileDescriptor);
            ouputTextFromFile(fileName);
            free(fileName);

            // readFromFileIntoConsole(filePointer);

            close(fileDescriptor);
            break;
        }
    default:
        //parent
        {
            char* fileName = createFileNameWithPostfix(argv, "_parent");
            int fileDescriptor = createReadOnlyFile(fileName);
            readFileIntoFile(filePointer, fileDescriptor);
            ouputTextFromFile(fileName);
            free(fileName);

            // readFromFileIntoConsole(filePointer);

            wait(NULL);
            close(fileDescriptor);

            break;
        }
    }
}

char* createFileNameWithPostfix(char** argv, char* postfix) {
    char* newFileName = calloc(strlen(argv[1]) + strlen(postfix), sizeof(char));
    strcat(newFileName, argv[1]);
    strcat(newFileName, postfix);

    return newFileName;
}

int createReadOnlyFile(char* newFileName) {
    int outFile = creat(newFileName, strtol("444", NULL, 8));
    return outFile;
}

void readFileIntoFile(int srcFile, int destFile) {
    char buffer;
    int readBytes;
    while ((readBytes = read(srcFile, &buffer, 1)) > 0) {
        write(destFile, &buffer, readBytes);
    }
}

void ouputTextFromFile(char* fileName) {
    int fd = open(fileName, O_RDONLY);

    char buffer[1024];
    printf("Output of %s is:\n", fileName);
    while (read(fd, &buffer, 1024) > 0) {
        printf("%s", buffer);
    }

    close(fd);
}

void readFromFileIntoConsole(int fd) {
    char buffer;
    while (read(fd, &buffer, 1) > 0) {
        printf("%c", buffer);
    }
}