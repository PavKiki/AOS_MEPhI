#include "task7.h"

int main() {
    int child_pid = fork();

    if (child_pid) {
    //parent
        printf("It's a parent process, pid : %d\n", getpid());

        sleep(3);
        
        kill(child_pid, SIGUSR2);

        int status;
        pid_t waitCode = wait(&status);

        printf("\nChild exitcode is %d\n", WEXITSTATUS(status));
    } else {
    //child
        printf("It's a child process, pid : %d\n", getpid());

        pause();
    }
}