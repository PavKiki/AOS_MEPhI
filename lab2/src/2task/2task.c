#include "2task.h"

int main() {
    pid_t code = fork();

    code ? printf("PID of parent process is %d\n", getpid())
         : printf("PID of child process is %d\n", getpid());

    printf("PID of current process is %d\n\n", getpid());
}