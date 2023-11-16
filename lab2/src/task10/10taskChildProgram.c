#include "../lab2.h"

int main(int argc, char** argv, char** envp) {
    printf("Hello from child process in the execv() program!\n");
    printf("List of arguments:\n");
    for (int i = 0; i < argc; i++) printf("%s\n", argv[i]);
    printf("End of list of arguments\n");
    printf("List of enviroment variables:\n");
    for (char** i = envp; *i != NULL; i += 1) printf("%s\n", *i);
    printf("End of list of arguments\n\n");
}