#include "../lab4.h"

void main(int argc, char* argv[]) {
    printf("Entered exec program\n");

    int channel1[2] = {0, 0};
    channel1[0] = atoi(argv[1]);
    channel1[1] = atoi(argv[2]);
    
    close(channel1[0]);
            
    char letter;
    while (read(STDIN_FILENO, &letter, 1) > 0) write(channel1[1], &letter, 1);
            
    close(channel1[1]);
}