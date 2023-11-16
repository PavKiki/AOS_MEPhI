#include "task10.h"

int main(int argc, char* argv[]) {
    int childPid = fork();
    switch (childPid) {
        case -1: {
            perror("Error on fork occured!");
            break;
        }
        case 0: {
            //child
            char letter;
            while(1) {
                read(STDIN_FILENO, &letter, 1);
                char* res = calloc(10, 1);
                strcat(res, "[CHILD] ");
                strcat(res, &letter);
                strcat(res, "\n");
                write(STDOUT_FILENO, res, 10);
            }
            break;
        }
        default: {
            //parent
            char letter;
            while(1) {
                read(STDIN_FILENO, &letter, 1);
                char* res = calloc(11, 1);
                strcat(res, "[PARENT] ");
                strcat(res, &letter);
                strcat(res, "\n");
                write(STDOUT_FILENO, res, 11);
            }
            break;
        }
    }
}