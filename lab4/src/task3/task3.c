#include "task3.h"

void customHandler(int signum) {
    printf("Hello from handler!\n");
}

int main() {
    int channel1[2];
    int channel2[2];
    //0 - read, 1 - write
    if (pipe(channel1) < 0) perror("");
    if (pipe(channel2) < 0) perror("");

    int childPid = fork();
    switch (childPid) {
        case -1: {
            perror("Error on fork occured!");
            break;
        }
        case 0: {
            //child
            close(channel1[0]);
            close(channel2[1]);
            
            while (1) {
                char letter1;
                if (read(STDIN_FILENO, &letter1, 1) > 0) write(channel1[1], &letter1, 1);
                printf("[CHILD] Info was read from STDIN and written to the first channel\n");

                char letter2;
                if (read(channel2[0], &letter2, 1) > 0) write(STDOUT_FILENO, &letter2, 1);
                printf("[CHILD] Info was read from the second channel and written to the STDOUT channel\n");
            }

            close(channel1[1]);
            close(channel2[0]);
            break;
        }
        default: {
            //parent
            close(channel1[1]);
            close(channel2[0]);
            
            while (1) {
                char letter1;
                if (read(channel1[0], &letter1, 1) > 0) write(STDOUT_FILENO, &letter1, 1);
                printf("[PARENT] Info was read from the first channel and written to the STDOUT channel\n");

                char letter2;
                if (read(STDIN_FILENO, &letter2, 1) > 0) write(channel2[1], &letter2, 1);
                printf("[PARENT] Info was read from STDIN and written to the second channel\n");
            }

            close(channel1[0]);
            close(channel2[1]);

            wait(NULL);
            break;
        }
    }
}