#include "10task.h"

int main(int argc, char** argv, char** envp) {
    int pid_child = fork();

    switch (pid_child) {
        case 0: {
            //child
            // char* executableFile = "./home/paulo/UCHEBA/\"Архитектура операционных систем\"/lab2/out/childProgramTask10";
            execv(argv[1], argv + 1);
            break;    
        }
        default: {
            //parent
            // execlp("pwd", "pwd", NULL);

            printf("Hello from parent process in the original program!\n");
            printf("List of arguments:\n");
            for (int i = 0; i < argc; i++) printf("%s\n", argv[i]);
            printf("End of list of arguments\n");
            printf("List of enviroment variables:\n");
            for (char** i = envp; *i != NULL; i += 1) printf("%s\n", *i);
            printf("End of list of arguments\n\n");

            wait(NULL);
            break;
        }
    }
}