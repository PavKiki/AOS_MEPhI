#include "task11.h"

void newHandler(int signum) {
    printf("Received signal from pid %d\n", getpid());
}

//it is not working, repair it
int main() {
    sigset_t all;
    sigset_t sigusr;
    sigemptyset(&all);
    sigemptyset(&sigusr);
    sigaddset(&sigusr, SIGUSR1);

    sigprocmask(SIG_BLOCK, &sigusr, &all);

    signal(SIGUSR1, newHandler);

    int child_pid = fork();
    if (child_pid) {
    //parent
        printf("It's a parent process, pid : %d\n", getpid());

        
        kill(child_pid, SIGUSR1);
        sigsuspend(&all);
    } else {
    //child
        printf("It's a child process, pid : %d\n", getpid());

        for (int i = 0; i < 10000; i++) {
            printf("%d ", i);
            for (int j = 0; j < 100000; j++);
        }

        printf("\n");

        kill(getppid(), SIGUSR1);
        sigsuspend(&all);
    }
}