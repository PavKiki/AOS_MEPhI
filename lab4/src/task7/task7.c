#include "task7.h"

int main(int argc, char* argv[]) {
	struct stat st;
    char* file = argv[1];
    if (mkfifo(file, 0777) == -1) perror("mkfifo");
    if (stat(file, &st) == -1) perror("fifo");
	if (!S_ISFIFO(st.st_mode)) printf("%s file is not a fifo\n", file);
    else printf("%s file is a fifo\n", file);

    int childPid = fork();
    switch (childPid) {
        case -1: {
            perror("Error on fork occured!");
            break;
        }
        case 0: {
            char letter;
            int readDescriptor = open(file, O_RDONLY);
            perror("readDescriptor");

            int k = 0;
            while (read(readDescriptor, &letter, 1) > 0) {
                k++;
            }

            printf("Output: %d\n", k);

            close(readDescriptor);
            break;
        }
        default: {
            //parent
            int writeDescriptor = open(file, O_WRONLY);
            perror("writeDescriptor");
            
            for (int i = 0; i < 10000000; i++) write(writeDescriptor, "A", 1);

            close(writeDescriptor);
            wait(NULL);
            break;
        }
    //https://www.opennet.ru/man.shtml?topic=fcntl&category=2
    //error will occure
    }
}