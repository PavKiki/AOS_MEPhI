#include "3task.h"

//https://www.opennet.ru/man.shtml?topic=waitpid&category=2&russian=0
//http://www.qnx.com/developers/docs/qnxcar2/index.jsp?topic=%2Fcom.qnx.doc.neutrino.lib_ref%2Ftopic%2Fw%2Fwaitpid.html
int main() {
    pid_t code = fork();

    if (code) {
        printf("PID of parent process is %d\n", getpid());

        int status;
        pid_t waitCode = wait(&status);

        printf("Child exitcode is %d, not converted is %d\n", WEXITSTATUS(status), status);
    } else {
        printf("PID of child process is %d\n", getpid());
        return 100;
    } 

    printf("This process is about to be terminated - PID = %d ; PPID = %d ; PGID = %d\n", getpid(), getppid(), getpgrp());
    return 1;

}