#include "6task.h"

int main() {
    pid_t code = fork();

    if (code) {
        int status;
        pid_t waitCode = wait(&status);

        printf("Child exitcode is %d\n", WEXITSTATUS(status));
        
        printf("[PARENT] PID = %d ; PPID = %d ; PGID = %d\n", getpid(), getppid(), getpgrp());
        pause();
    } else {
        printf("[CHILD] PID = %d ; PPID = %d ; PGID = %d\n", getpid(), getppid(), getpgrp());
        pause();
    } 

    //Ctrl-C = SIGINT (Unix stop process signal)
    return 1;

}