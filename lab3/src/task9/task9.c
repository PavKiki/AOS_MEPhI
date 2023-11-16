#include "task9.h"

void newHandler() {
    printf("Received signal!\n");
}

int main() {
    int child_pid = fork();

    if (child_pid) {
    //parent
        printf("It's a parent process, pid : %d\n", getpid());

        int status;
        pid_t waitCode = wait(&status);

        printf("\nChild exitcode is %d\n", WEXITSTATUS(status));
    } else {
    //child
        printf("It's a child process, pid : %d\n", getpid());

        signal(SIGALRM, newHandler);
        alarm(1);

        for (int i = 0; i < 10000; i++) {
            printf("%d ", i);
            for (int j = 0; j < 100000; j++);
        }

        printf("\n");
        //loop is not interrupted by SIGALRM if there is a handler
        //and interrupted if there is no handler
        //by default it is terminates the program
    }
}