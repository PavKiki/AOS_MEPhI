#include "5task.h"

int main() {
    pid_t code = fork();

    if (code) {
        printf("PID of parent process is %d\n", getpid());
        sleep(3);
        system("ps -efj");
        //ps aux | grep ...
    } else {
        printf("PID of child process is %d\n", getpid());
    } 

    //when the parent is killed, it will be reassigned to 1000, which is waits for it's signal
    printf("This process is about to be terminated - PID = %d ; PPID = %d ; PGID = %d\n", getpid(), getppid(), getpgrp());
    return 1;
}