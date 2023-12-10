#include "task9.h"

int main(int argc, char* argv[]) {
    char* file = "notfifo.txt";
    creat(file, 0777);

    struct flock wlock, rlock, unlock;

    wlock.l_type=F_WRLCK;
	wlock.l_whence=SEEK_SET;
	wlock.l_start=0;
	wlock.l_len=0;

    rlock.l_type=F_RDLCK;
	rlock.l_whence=SEEK_SET;
	rlock.l_start=0;
	rlock.l_len=0;

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

            struct flock tmpLock;
            do {
                fcntl(readDescriptor, F_GETLK, &tmpLock);
                sleep(0.5);
            } while(tmpLock.l_type == F_UNLCK);
            while(read(readDescriptor, &letter, 1) > 0) printf("%c\n", letter);
            // fcntl(readDescriptor, F_SETLK, &unlock);

            close(readDescriptor);
            break;
        }
        default: {
            //parent
            int writeDescriptor = open(file, O_WRONLY);
            perror("writeDescriptor");
            
            fcntl(writeDescriptor, F_SETLK, &wlock);
            for (int i = 0; i < 1000000; i++) write(writeDescriptor, "A", 1);
            fcntl(writeDescriptor, F_SETLK, &unlock);

            close(writeDescriptor);
            wait(NULL);
            break;
        }
    }
}