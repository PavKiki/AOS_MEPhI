#include "task4.h"

// 4.
// Модифицировать программу п. 2, позволив ей принимать первые несколько байтов сообщений произвольных размеров.

int main(int argc, char** argv) {
    int identifier = atoi(argv[1]);
    int type = atoi(argv[2]);

    printMessageQueueInfo(identifier);

    for (size_t i = 1; i <= 10; i++) {
        ShortMessage msg;
        
        //there will not be errors, if actual size of message data is bigger than provided buffer
        if (msgrcv(identifier, &msg, sizeof(msg.data), type, IPC_NOWAIT | MSG_NOERROR) == -1) {
            perror("Unable to read message from the queue");
        }
        else {
            printf("Message of type %d was successfully read!\n", type);
            printf("%s", msg.data);
        }
    }

    printMessageQueueInfo(identifier);
}