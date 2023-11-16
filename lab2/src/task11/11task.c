#include "11task.h"

int main(int argc, char** argv) {
    int pid_child = fork();

    switch (pid_child) {
        case 0: {
            //child
            printf("Child process executes via execvp():\n");
            execvp(argv[1], argv + 1);
            break;    
        }
        default: {
            //parent
            printf("Parent process executes like a man:\n");
            system("ls -l");

            wait(NULL);
            break;
        }
    }
}