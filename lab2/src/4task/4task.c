#include "4task.h"

int main() {
    pid_t code = fork();

    if (code) {
        printf("PID of parent process is %d\n", getpid());

        int status;
        printf("Child exitcode is %d\n", WEXITSTATUS(status));
    } else {
        printf("Parent PID before sleep is %d\n", getppid());
        printf("PID of child process is %d\n", getpid());

        sleep(30);
        printf("Parent PID after sleep is %d\n", getppid());
    } 

    printf("This process is about to be terminated - PID = %d ; PPID = %d ; PGID = %d\n", getpid(), getppid(), getpgrp());
    return 1;

    //https://askubuntu.com/questions/1420764/why-this-orphaned-process-is-called-by-a-child-process-of-init-instead-of-init
    //1000 because paulo       1000       1    1000    1000  0 20:40 ?        00:00:00 /lib/systemd/systemd --user
}