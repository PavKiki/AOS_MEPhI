#include "task1.h"

void signalProcessing(int signum) {
    printf("The reaction on Ctrl-C was changed\n");
    signal(signum, SIG_DFL);
    //signal is blocked
    sleep(10);
}

int main() {
    signal(SIGINT, signalProcessing);
    printf("Before sleep\n");
    sleep(10);
    printf("After handling\n");
}
