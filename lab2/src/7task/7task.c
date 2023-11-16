#include "7task.h"

int main() {
    pid_t code = fork();

    if (code) {
        int status;
        pid_t waitCode = wait(&status);

        printf("Child exitcode is %d\n", WEXITSTATUS(status));
        
        //anyway will be inside one session, just in another group
        printf("[PARENT] PID = %d ; PPID = %d ; PGID = %d\n", getpid(), getppid(), getpgrp());

        pause();
    } else {
        printf("[CHILD] PID = %d ; PPID = %d ; PGID = %d\n", getpid(), getppid(), getpgrp());
        setpgrp(); //setpgid(0, 0)
        printf("[CHILD] PID = %d ; PPID = %d ; PGID = %d\n", getpid(), getppid(), getpgrp());
        
        pause();
    } 

    //Ctrl-C = SIGINT (Unix stop process signal)

    //no, it wouldn't be affected after Ctrl-C
    //yes, it will be displayed in ps
    return 1;
}