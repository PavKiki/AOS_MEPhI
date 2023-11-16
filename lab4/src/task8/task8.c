#include "task8.h"

int main(int argc, char* argv[]) {
    char* file = "notfifo.txt";
    creat(file, 0777);

    struct flock lock, unlock;

    lock.l_type=F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=0;
	lock.l_len=0;

    unlock.l_type=F_UNLCK;

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

            int status = 1000;
            sleep(0.5);
            while(status = read(readDescriptor, &letter, 1) > 0) printf("%c\n", letter);
            perror("perror");
            printf("%d", &status);

            close(readDescriptor);
            break;
        }
        default: {
            //parent
            int writeDescriptor = open(file, O_WRONLY);
            perror("writeDescriptor");
            
            fcntl(writeDescriptor, F_SETLK, &lock);
            for (int i = 0; i < 100000; i++) write(writeDescriptor, "A", 1);
            fcntl(writeDescriptor, F_SETLK, &unlock);

            close(writeDescriptor);
            wait(NULL);
            break;
        }
    }
}