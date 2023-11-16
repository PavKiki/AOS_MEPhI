#include "task11.h"

int main(int argc, char* argv[]) {
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
            //child
            while(1) {
                fcntl(STDIN_FILENO, F_SETLKW, &lock);
                
                for (int i = 0; i < 5; i++) {
                    char letter;
                    read(STDIN_FILENO, &letter, 1);
                    char* res = calloc(10, 1);
                    strcat(res, "[CHILD] ");
                    strcat(res, &letter);
                    write(STDOUT_FILENO, res, 10);
                }

                fcntl(STDIN_FILENO, F_SETLK, &unlock);
            }
            break;
        }
        default: {
            //parent
            while(1) {
                fcntl(STDIN_FILENO, F_SETLKW, &lock);

                for (int i = 0; i < 5; i++) {
                    char letter;
                    read(STDIN_FILENO, &letter, 1);
                    char* res = calloc(11, 1);
                    strcat(res, "[PARENT] ");
                    strcat(res, &letter);
                    write(STDOUT_FILENO, res, 11);
                }

                fcntl(STDIN_FILENO, F_SETLK, &unlock);
                sleep(0.5);
            }
            break;
        }
    }
}