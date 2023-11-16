#include "task1.h"

int main() {
    int channel1[2];
    int channel2[2];
    //0 - read, 1 - write
    if (pipe(channel1) < 0) perror("");
    if (pipe(channel2) < 0) perror("");

    int isDuplex;
    printf("Do you want to create duplex connection? (0 - no, 1 - yes)\n> ");
    scanf("%d", &isDuplex);

    char letter;

    int childPid = fork();
    switch (childPid) {
        case -1: {
            perror("Error on fork occured!");
            break;
        }
        case 0: {
            //child
            close(channel1[0]);
            
            write(channel1[1], "A", 1);
            
            if (isDuplex) {
                close(channel2[1]);

                sleep(2);
                read(channel2[0], &letter, 1);
                printf("%c\n", letter);
            }
            
            close(channel1[1]);
            close(channel2[0]);
            break;
        }
        default: {
            //parent
            close(channel1[1]);
            
            sleep(1);
            read(channel1[0], &letter, 1);
            printf("%c\n", letter);
            
            if (isDuplex) {
                close(channel2[0]);
                
                write(channel2[1], "B", 1);
            }
            
            close(channel1[0]);
            close(channel2[1]);

            wait(NULL);
            break;
        }
    }
}