#include "task3.h"

void signalProcessing() {
    printf("Children will die immediately\n");
}

int main() {
    int child_pid = fork();

    if (child_pid) {
    //parent
        printf("It's a parent process, pid : %d\n", getpid());

        struct sigaction customHandler;
        customHandler.sa_handler = signalProcessing;
        sigemptyset(&customHandler.sa_mask);
        customHandler.sa_flags = SA_NOCLDWAIT;

        //so many questions
        if (sigaction(SIGCHLD, &customHandler, NULL) < 0) perror("");
        system("ps");
    } else {
    //child
        printf("It's a child process, pid : %d\n", getpid());
    }
}
