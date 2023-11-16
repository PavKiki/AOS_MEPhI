#include "task5.h"

int main() {
    int child_pid = fork();

    if (child_pid) {
    //parent
        printf("It's a parent process, pid : %d\n", getpid());
        
        // kill(child_pid, SIGUSR1);       //task5

        kill(child_pid, SIGSTOP);       //task6
        kill(child_pid, SIGCONT);       //task6

        int status;
        pid_t waitCode = wait(&status);

        printf("\nChild exitcode is %d\n", WEXITSTATUS(status));
    } else {
    //child
        printf("It's a child process, pid : %d\n", getpid());

        for (int i = 0; i < 100; i++) {
            printf("%d ", i);
            for (int j = 0; j < 100; j++) {
            }
        }
    }
}