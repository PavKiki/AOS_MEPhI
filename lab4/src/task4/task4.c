#include "task4.h"

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
            printf("[CHILD] Execution of child started\n");

            char arg1[11], arg2[11];
            sprintf(arg1, "%d", channel1[0]);
            sprintf(arg2, "%d", channel1[1]);
            system("pwd");
            execl("./lab4_", arg1, arg2);
            break;
        }
        default: {
            //parent
            printf("[PARENT] Execution of parent started\n");

            close(channel1[1]);
            
            char letter;
            while (read(channel1[0], &letter, 1) > 0) write(STDOUT_FILENO, &letter, 1);

            close(channel1[0]);

            wait(NULL);
            break;
        }
    }
}