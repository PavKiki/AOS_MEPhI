#include "task2.h"

void signalProcessing() {
    struct sigaction oldHandler;
    oldHandler.sa_handler = SIG_DFL;
    sigemptyset(&oldHandler.sa_mask);
    oldHandler.sa_flags = 0;

    printf("The reaction on Ctrl-C was changed\n");
    if (sigaction(SIGINT, &oldHandler, NULL) < 0) perror("");
    sleep(10);
}

// TERMINAL/SAMOS ONLY, terminal in the vs code doesn't work
int main() {
    struct sigaction newHandler;
    newHandler.sa_handler = signalProcessing;
    sigemptyset(&newHandler.sa_mask);
    newHandler.sa_flags = 0;

    if (sigaction(SIGINT, &newHandler, NULL) < 0) perror("");
    printf("Before sleep\n");
    sleep(10);
}