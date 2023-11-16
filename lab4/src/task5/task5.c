#include "task5.h"

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

            dup2(channel1[1], STDOUT_FILENO);
            close(channel1[1]);

            system("who");

            break;
        }
        default: {
            //parent
            close(channel1[1]);
            
            dup2(channel1[0], STDIN_FILENO);
            close(channel1[0]);

            system("wc -l");

            wait(NULL);
            break;
        }
    }
}