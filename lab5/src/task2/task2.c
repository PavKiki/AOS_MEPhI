#include "task2.h"

// 2. 
// Написать программу, позволяющую читать сообщения из очереди и выводить их на экран. 
// Идентификатор очереди и тип сообщения передается через аргументы командной строки.

// 3. 
// Модифицировать программу п. 2, позволив ей избежать ожидания в случае отсутствия в очереди сообщений данного типа.

int main(int argc, char** argv) {
    int identifier = atoi(argv[1]);
    int type = atoi(argv[2]);

    printMessageQueueInfo(identifier);

    for (size_t i = 1; i <= 10; i++) {
        Message msg;
        
        //IPC_NOWAIT stands for 3
        if (msgrcv(identifier, &msg, sizeof(msg.data), type, IPC_NOWAIT) == -1) {
            perror("Unable to read message from the queue");
        }
        else {
            printf("Message of type %d was successfully read!\n", type);
            printf("%s", msg.data);
        }
    }

    printMessageQueueInfo(identifier);
}