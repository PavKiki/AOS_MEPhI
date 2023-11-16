#include "task2.h"

int main() {
    int channel1[2];
    //0 - read, 1 - write
    if (pipe(channel1) < 0) perror("");

    int childPid = fork();
    switch (childPid) {
        case -1: {
            perror("Error on fork occured!");
            break;
        }
        case 0: {
            //child
            close(channel1[0]);
            
            char letter;
            while (read(STDIN_FILENO, &letter, 1) > 0) write(channel1[1], &letter, 1);
            
            close(channel1[1]);
            break;
        }
        default: {
            //parent
            close(channel1[1]);
            
            char letter;
            while (read(channel1[0], &letter, 1) > 0) write(STDOUT_FILENO, &letter, 1);

            close(channel1[0]);

            wait(NULL);
            break;
        }
    }
}