#include "task1.h"

// 1. 
// Создать очередь сообщений или получить доступ к уже существующей очереди. 
// Вывести на экран статистическую информацию об очереди сообщений. 
// Поместить несколько сообщений различных типов в очередь.

int main() {
    int identifier;
    //IPC_PRIVATE - every time new resource and identifier
    //IPC_EXCL - to take care if identifier already exists
    if ((identifier = msgget(IPC_PRIVATE, 0666 | IPC_CREAT | IPC_EXCL)) == -1) perror("Cannot create new identifier");
    else printf("New identifier is %d\n", identifier);

    printMessageQueueInfo(identifier);

    for (size_t i = 1; i <= 10; i++) {
        Message msg;
        msg.type = i;
        sprintf(msg.data, "Hello from type %d!", i);

        //IPC_NOWAIT is set, so on running out of buffer msgsnd will be blocked
        if (msgsnd(identifier, &msg, sizeof(msg.data), IPC_NOWAIT) == -1) perror("Error on sending message to the queue");
    }
}