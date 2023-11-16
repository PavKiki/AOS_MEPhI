#include "task10.h"

//ONLY TERMINAL, NO CONSOLE IN VS CODE!!!!!!
void signalProcessing() {
    printf("Received signal!\n");
    sleep(10);
}

int main() {
    struct sigaction newHandler;
    newHandler.sa_handler = signalProcessing;
    newHandler.sa_flags = 0;
    sigemptyset(&newHandler.sa_mask);
    sigaddset(&newHandler.sa_mask, SIGINT);

    if (sigaction(SIGALRM, &newHandler, NULL) < 0) perror("");

    alarm(1);

    alarm(1);

    for (int i = 0; i < 100000; i++) {
        printf("%d ", i);
        for (int j = 0; j < 100000; j++);
    }

    printf("\n");
}